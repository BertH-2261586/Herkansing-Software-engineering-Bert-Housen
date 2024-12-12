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
        self.group_manager = GroupManager(session)

    def create_user(self, user: User) -> User:
        """Creates a user in the Database"""
        self.session.add(user)
        self.session.commit()
        self.session.refresh(user)
        return user
    
    def get_user(self, user: UserBase) -> User:
        """Gets user from database based on username"""
        return self.session.exec(select(User).filter_by(username=user.username)).first()

    def remove_user(self, user_id):
        """Removes user with given id"""
        user = self.session.exec(select(User).filter_by(id=user_id)).first()

        if user:
            self.group_manager.remove_owned_groups(user_id)

            self.session.delete(user)
            self.session.commit()

            return True
        else:
            return False

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


