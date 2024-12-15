from sqlmodel import create_engine, Session, select
from .models import User, UserBase
from typing import Optional

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
    def get_users_by_page(self, page: int, page_size: int = 10, search_query: Optional[str] = None) -> list[str]:
        offset_value = (page - 1) * page_size                                    # Calculate from what row to start
        query = select(User.username).offset(offset_value).limit(page_size)      # Generate the query
        if search_query:
            # If a search query is provided, filter the users by username (case insensitive)
            query = query.filter(User.username.ilike(f"%{search_query}%"))       

        return self.session.scalars(query).all()


class InboxManager:
    def __init__(self, session : Session):
        self.session = session

    
class FriendManager:
    def __init__(self, session : Session):
        self.session = session
    