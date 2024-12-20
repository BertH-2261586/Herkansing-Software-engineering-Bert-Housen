import logging
from fastapi import Request
from sqlmodel import create_engine, Session, select
from .models import User, UserBase, Group, GroupInvite, GroupMember, Inbox, Friend
from sqlalchemy import func  
from typing import Optional, Tuple, List
import sys

DATABASE_URL = "mysql+pymysql://user:password@mysql-db:3306/dbname"
logging.basicConfig(level=logging.DEBUG, stream=sys.stdout, format='%(asctime)s - %(levelname)s - %(message)s')

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
        
    def get_users_by_page(self, page: int, page_size: int = 10, search_query: Optional[str] = None) -> Tuple[int, List[str]]:
        offset_value = (page - 1) * page_size                                    # Calculate from what row to start
        query = select(User.username).offset(offset_value).limit(page_size)      # Generate the query
        if search_query:
            query = query.filter(User.username.ilike(f"%{search_query}%"))  
        
        # Get the list of usernames for the current page
        user_list = self.session.scalars(query).all()               
        
        # Get the total amount of users found
        count_query = select(func.count(User.username))             
        total_count = self.session.scalar(count_query)

        return total_count, user_list
    
    def get_userID_via_username(self, username: str) -> Optional[int]:
        query = select(User.id).where(User.username == username)
        user_id = self.session.scalar(query)
        return user_id

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


    
    # Fetches users in a specific range based on page number and page size
    def get_users_by_page(self, page: int, page_size: int = 10, search_query: Optional[str] = None) -> Tuple[int, List[str]]:
        offset_value = (page - 1) * page_size                                    # Calculate from what row to start
        query = select(User.username).offset(offset_value).limit(page_size)      # Generate the query
        if search_query:
            # If a search query is provided, filter the users by username (case insensitive)
            query = query.filter(User.username.ilike(f"%{search_query}%"))  
        
        # Get the list of usernames for the current page
        user_list = self.session.scalars(query).all()               
        
        # Get the total amount of users found
        count_query = select(func.count(User.username))             
        total_count = self.session.scalar(count_query)

        return total_count, user_list
    
    def get_userID_via_username(self, username: str) -> Optional[int]:
        query = select(User.id).where(User.username == username)
        user_id = self.session.scalar(query)
        return user_id

class InboxManager:
    def __init__(self, session : Session):
        self.session = session

    # Add item to inbox
    def create_inbox_item(self, inbox: Inbox):
        self.session.add(inbox)
        self.session.commit()
        self.session.refresh(inbox)
    
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