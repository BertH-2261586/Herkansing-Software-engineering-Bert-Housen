from sqlmodel import SQLModel, Field
from typing import Optional

# User Model
class UserBase(SQLModel):
    username: str = Field(min_length=3, max_length=50)
    password: str = Field(min_length=8, max_length=255)

class UserLogin(SQLModel):
    username: str
    password: str

# Extend the userBase with an ID, this represents the User table
class User(UserBase, table=True):
    id: Optional[int] = Field(default=None, primary_key=True)

class Friend(SQLModel, table=True):
    id: Optional[int] = Field(default=None, primary_key=True)
    user1: int = Field(foreign_key="user.id")   
    user2: int = Field(foreign_key="user.id")

class Inbox(SQLModel, table=True):
    id: Optional[int] = Field(default=None, primary_key=True)
    type: str                                        # Friend request or questionset
    sending_user: int = Field(foreign_key="user.id")
    receiving_user: int = Field(foreign_key="user.id")
    # message: str         uncomment if you want to add a message option for the user
    # questionSets: Optional[int] = Field(default=None)