from sqlmodel import create_engine, Session, select
from .models import User, UserBase, Group

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

    def create_user(self, user: User) -> User:
        """Creates a user in the Database"""
        self.session.add(user)
        self.session.commit()
        self.session.refresh(user)
        return user
    
    def get_user(self, user: UserBase) -> User:
        """Gets user from database based on username"""
        return self.session.exec(select(User).filter_by(username=user.username)).first()

class GroupManager:
    def __init__(self, session : Session):
        self.session = session
    
    def create_group(self, group : Group) -> Group:
        """Creates group in the database"""
        self.session.add(group)
        self.session.commit()
        self.session.refresh(group)
        return group

