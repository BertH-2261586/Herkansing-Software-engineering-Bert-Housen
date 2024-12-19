from sqlmodel import SQLModel, Field
from typing import Optional
from pydantic import BaseModel


# JSON validation models

class QuestionSetShare(BaseModel):
    code: str
    id: int
    friend_ids: list[int]


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



class FriendBase(SQLModel):
    user1: int = Field(foreign_key="user.id")   
    user2: int = Field(foreign_key="user.id")

class Friend(FriendBase, table=True):
    id: Optional[int] = Field(default=None, primary_key=True)



class InboxBase(SQLModel):
    type: str                                   
    sending_user: int = Field(foreign_key="user.id")   
    receiving_user: int = Field(foreign_key="user.id") 
    code: Optional[str] = Field(default=None)  
    # message: str         uncomment if you want to add a message option for the user    
    
class Inbox(InboxBase, table=True):
    id: Optional[int] = Field(default=None, primary_key=True)