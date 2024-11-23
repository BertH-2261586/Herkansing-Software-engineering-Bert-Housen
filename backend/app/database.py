from sqlmodel import create_engine, Session
from .models import User

DATABASE_URL = "mysql+pymysql://user:password@mysql-db:3306/dbname"

# Create the database engine
engine = create_engine(DATABASE_URL, echo=True)

# Dependency to get a database session
def get_session():
    with Session(engine) as session:
        yield session

def create_user(session: Session, user: User) -> User:
    session.add(user)
    session.commit()
    session.refresh(user)
    return user
