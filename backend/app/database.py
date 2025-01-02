from sqlmodel import create_engine, Session, select
from .models import User, UserBase, Group, GroupInvite, GroupMember, Inbox, Friend, GroupCodeInvite, QuestionSet
from sqlalchemy import func, or_  
from typing import Optional, Tuple, List
from datetime import datetime, timezone

DATABASE_URL = "mysql+pymysql://user:password@mysql-db:3306/dbname"

# Create the database engine
engine = create_engine(DATABASE_URL, echo=True)

# Dependency to get a database session
def get_session():
    with Session(engine) as session:
        yield session

class UserManager:
    def __init__(self, session : Session):
        self.session = session
        self.group_manager = GroupManager(session)

    # Creates a user in the Database
    def create_user(self, user: User) -> User:
        self.session.add(user)
        self.session.commit()
        self.session.refresh(user)
        return user
    
    # Gets user from database based on username
    def get_user(self, user: UserBase) -> User:
        return self.session.exec(select(User).filter_by(username=user.username)).first()

    def remove_user(self, user_id):
        """Removes user with given id"""
        user = self.session.exec(select(User).filter_by(id=user_id)).first()

        if user:
            #self.group_manager.remove_owned_groups(user_id)

            self.session.delete(user)
            self.session.commit()

            return True
        else:
            return False
        
    def get_user_invites(self, user_id):
        """Gets all user invites"""
        return self.session.exec(select(User).filter_by(id=user_id)).first().group_invites
    
    def accept_invite(self, invite_id):
        """Accepts invite by certain id"""
        invite = self.session.exec(select(GroupInvite).filter_by(id=invite_id)).first()

        if invite:
            self.session.add(GroupMember(group_id=invite.group_id, user_id=invite.user_id))
            self.session.delete(invite)
            self.session.commit()
            
            return True
        else:
            return False
    
    def reject_invite(self, invite_id):
        """Rejects invite by certain id"""
        invite = self.session.exec(select(GroupInvite).filter_by(id=invite_id)).first()

        if invite:
            self.session.delete(invite)
            self.session.commit()
            
            return True
        else:
            return False
    
    def has_invite_by_id(self, user_id, invite_id):
        """Checks if user has a invite by given id"""
        invite = self.session.exec(select(GroupInvite).filter_by(id=invite_id, user_id=user_id)).first()

        if invite:
            return True
        else:
            return False
        
    def get_users_by_page(self, userID: int, page: int, page_size: int, 
                          search_query: Optional[str] = None) -> Tuple[int, int, List[str]]:
        offset_value = (page - 1) * page_size                                                   # Calculate from what row to start
        query = select(User.id, User.username).offset(offset_value).limit(page_size).where(User.id != userID)   # Generate the query
        if search_query:
            query = query.filter(User.username.ilike(f"%{search_query}%"))  
        
        # Get the list of usernames for the current page
        results = self.session.exec(query).all()
        user_IDs = [row.id for row in results]
        user_list = [row.username for row in results]

        # Get the total amount of users found
        count_query = select(func.count(User.username)).where(User.id != userID)
        if search_query:
            count_query = count_query.filter(User.username.ilike(f"%{search_query}%"))          
        total_count = self.session.scalar(count_query)

        return total_count, user_IDs, user_list
    
    def get_userID_via_username(self, username: str) -> Optional[int]:
        query = select(User.id).where(User.username == username)
        user_id = self.session.scalar(query)
        return user_id
    
    def get_usernames_from_IDs(self, user_IDs: list[int]) -> list[str]:
        # Get all the usernames given a list of IDs
        return self.session.exec(select(User.username).where(User.id.in_(user_IDs))).all()
    
class GroupManager:
    def __init__(self, session : Session):
        self.session = session
    
    def create_group(self, group : Group) -> Group:
        """Creates group in the database"""
        self.session.add(group)
        self.session.commit()
        self.session.refresh(group)
        return group

    def remove_group(self, group_id):
        """Removes user with given id"""
        group = self.session.exec(select(Group).filter_by(id=group_id)).first()

        if group:
            self.session.delete(group)
            self.session.commit()

            return True
        else:
            return False
    
    def remove_owned_groups(self, user_id):
        """Removes all groups which user is owner of"""
        groups = self.session.exec(select(Group).filter_by(owner_id=user_id)).all()

        for group in groups:
            self.session.delete(group)
        self.session.commit()

    def is_group_owner(self, user_id, group_id):
        """returns true if user is group owner"""
        group = self.session.exec(select(Group).filter_by(owner_id=user_id, id=group_id)).first()

        if group:
            return True
        else:
            return False
    
    def invite_user(self, group_id, user_id):
        """Invites user to group returns true if success"""
        group = self.session.exec(select(Group).filter_by(id=group_id)).first()
        user = self.session.exec(select(User).filter_by(id=user_id)).first()

        if group and user:
            self.session.add(GroupInvite(group_id=group_id, user_id=user_id))
            self.session.commit()
            return True
        
        return False

    def get_group_by_code(self, codeToCheck) -> Group:
        """Gets group associated with code"""
        code_invitation = self.session.exec(select(GroupCodeInvite).filter_by(code=codeToCheck)).first()

        if code_invitation:
            if code_invitation.expires.replace(tzinfo=timezone.utc) > datetime.now(timezone.utc):
                return self.session.exec(select(Group).filter_by(id=code_invitation.group_id)).first()
            else:
                self.remove_group_code(code_invitation.id)  #delete code from db if expired
        else:
            return

    def remove_group_code(self, group_code_id):
        """Removes group code with given id"""
        group_code = self.session.exec(select(GroupCodeInvite).filter_by(id=group_code_id)).first()

        if group_code_id:
            self.session.delete(group_code)
            self.session.commit()

    def get_group_code(self, group_id_to_check):
        """Saves code as group invitation code"""
        code_invitation = self.session.exec(select(GroupCodeInvite).filter_by(group_id=group_id_to_check)).first()

        if code_invitation:
            if code_invitation.expires.replace(tzinfo=timezone.utc) > datetime.now(timezone.utc):
                return code_invitation.code
            else:
                self.remove_group_code(code_invitation.id)  #delete code from db if expired
        

    def save_group_code(self, code_to_save, group_id_to_save, expiration_date):
        """Saves code as group invitation code"""
        group_code = GroupCodeInvite(code=code_to_save, group_id=group_id_to_save, expires=expiration_date)
        self.session.add(group_code)
        self.session.commit()
    
    def join_group(self, user_id, group_id):
        """Lets user join group"""
        self.session.add(GroupMember(group_id=group_id, user_id=user_id))
        self.session.commit()

class InboxManager:
    def __init__(self, session : Session):
        self.session = session

    # Add item to inbox
    def create_inbox_item(self, inbox: Inbox):
        self.session.add(inbox)
        self.session.commit()
        self.session.refresh(inbox)
    
    def check_sent_friend_request_batch(self, current_userID: int, user_ids: list) -> dict:
        # Build the query to check both directions of friend requests (sent or received)
        query = select(Inbox).where(
            or_(
                (Inbox.sending_user == current_userID)  & (Inbox.receiving_user.in_(user_ids))      & (Inbox.type == "friend_request"),
                (Inbox.sending_user.in_(user_ids))      & (Inbox.receiving_user == current_userID)  & (Inbox.type == "friend_request")
            )
        )
        
        # Execute the query
        results = self.session.exec(query).all()
        
        # Prepare a dictionary to store friend request statuses for each user
        friend_request_status = {user_id: {"sent": False, "received": False} for user_id in user_ids}
        
        # Process the results to determine the status of the friend request (sent or received)
        for inbox_entry in results:
            if inbox_entry.sending_user == current_userID:
                friend_request_status[inbox_entry.receiving_user]["sent"] = True
            elif inbox_entry.receiving_user == current_userID:
                friend_request_status[inbox_entry.sending_user]["received"] = True

        return friend_request_status

    def get_inbox_items_user(self, user_ID: int) -> list[dict]:
        # Get all inbox messages and replace the sending_user ID with the username
        query = (
            select(Inbox, User.username)
            .join(User, Inbox.sending_user == User.id)       
            .where(Inbox.receiving_user == user_ID)
        )
        result = self.session.exec(query).all()
        
        # Transform results into a list of dictionaries with usernames
        inbox_items = []
        for inbox, username in result:
            inbox_dict = {
                "id": inbox.id,
                "type": inbox.type,
                "sending_user_ID": inbox.sending_user,
                "sending_user": username,               # Replaced sending user ID with username
                "code": inbox.code
            }
            inbox_items.append(inbox_dict)

        return inbox_items
    
    def remove_inbox_message(self, inbox_message_ID) -> bool:
        # Try to find the message by ID
        inbox_item = self.session.get(Inbox, inbox_message_ID)
        if inbox_item:
            # Delete the message
            self.session.delete(inbox_item)
            self.session.commit()  # Commit the transaction to apply changes
            return True
        return False
    
class FriendManager:
    def __init__(self, session : Session):
        self.session = session
    
    # Add friendship to db
    def create_friend_connection(self, friendship: Friend):
        self.session.add(friendship)
        self.session.commit()
        self.session.refresh(friendship)

    # Check if two users are friends
    def are_friends_batch(self, current_userID: int, user_ids: list) -> dict:
        # Build the query to check friendships for the given user_ids
        query = select(Friend).where(
            or_(
                (Friend.user1 == current_userID) & (Friend.user2.in_(user_ids)),
                (Friend.user1.in_(user_ids)) & (Friend.user2 == current_userID)
            )
        )
        
        # Execute the query
        results = self.session.exec(query).all()

        # Prepare a dictionary of friendship status for each user in user_ids
        friendship_status = {user_id: False for user_id in user_ids}
        
        # Mark the users who are friends with the current user as True
        for friend_relation in results:
            # If user1 or user2 is the current_userID, the other user is a friend
            if friend_relation.user1 == current_userID:
                friendship_status[friend_relation.user2] = True
            elif friend_relation.user2 == current_userID:
                friendship_status[friend_relation.user1] = True
        
        return friendship_status
    
    def get_all_friends(self, user_ID : int) -> list[int]:
        # Query all rows to find all the friends of the user
        results = self.session.exec(select(Friend).where((Friend.user1 == user_ID) | (Friend.user2 == user_ID))).all()

        # Find all the IDs of the friends
        friend_IDs = []
        for row in results:
            if row.user1 == user_ID:
                friend_IDs.append(row.user2)
            else:
                friend_IDs.append(row.user1)

        return friend_IDs

class QuestionSetManager:
    def __init__(self, session : Session):
        self.session = session
    
    # Creates a question set with a code in the Database
    def add_question_set(self, question_set: QuestionSet):
        self.session.add(question_set)
        self.session.commit()
        self.session.refresh(question_set)

    def code_exists(self, generated_code: str) -> bool:
        """Checks if a question set has this code"""
        if self.session.exec(select(QuestionSet).filter_by(code = generated_code)).first():
            return True
        else:
            return False
    
    def get_question_set(self, code_input: str) -> bytes:
        return self.session.exec(select(QuestionSet.content).filter_by(code = code_input)).first()