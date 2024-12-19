import logging
from fastapi import Request
from sqlmodel import create_engine, Session, select
from sqlalchemy import func  
from .models import User, UserBase, Inbox, Friend
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

    # Creates a user in the Database
    def create_user(self, user: User) -> User:
        self.session.add(user)
        self.session.commit()
        self.session.refresh(user)
        return user
    
    # Gets user from database based on username
    def get_user(self, user: UserBase) -> User:
        return self.session.exec(select(User).filter_by(username=user.username)).first()
    
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