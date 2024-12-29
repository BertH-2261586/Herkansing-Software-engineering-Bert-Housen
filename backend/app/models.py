from sqlmodel import SQLModel, Field, Relationship, ForeignKey
from typing import Optional, List
from pydantic import BaseModel
from datetime import datetime


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
    groups_owned: List["Group"] = Relationship(back_populates="owner", cascade_delete=True)
    group_invites: List["GroupInvite"] = Relationship(back_populates="invited_user", cascade_delete=True)

class UserIdInput(SQLModel):
    id: int

class UserSearchRequest(SQLModel):
    userID: int
    page: int = 1               # Default page is 1
    search: Optional[str] = None   # Search query is optional

class UsernameInput(SQLModel):
    username: str

#Group Model
class GroupCreate(SQLModel):
    name: str = Field(min_length=3, max_length=50, nullable=False)
    description: Optional[str] = Field(max_length=255)
    owner_id: int = Field(default=None)

class Group(GroupCreate, table=True):
    id: Optional[int] = Field(default=None, primary_key=True)
    owner_id: int = Field(default=None, foreign_key="user.id", ondelete="CASCADE")
    owner: User = Relationship(back_populates="groups_owned")

class GroupIdInput(SQLModel):
    id: int

class GroupInviteInput(SQLModel):
    group_id: int
    user_id: int

class GroupInviteIdInput(SQLModel):
    id: int

class GroupInvite(SQLModel, table=True):
    id: Optional[int] = Field(default=None, primary_key=True)
    group_id: int = Field(default=None, foreign_key="group.id", ondelete="CASCADE")
    user_id: int = Field(default=None, foreign_key="user.id", ondelete="CASCADE")

    invited_user: User = Relationship(back_populates="group_invites")

class GroupMember(SQLModel, table=True):
    id: Optional[int] = Field(default=None, primary_key=True)
    group_id: int = Field(default=None, foreign_key="group.id", ondelete="CASCADE")
    user_id: int = Field(default=None, foreign_key="user.id", ondelete="CASCADE")

class GroupCodeInvite(SQLModel, table=True):
    id: Optional[int] = Field(default=None, primary_key=True)
    code: str = Field(min_length=10, max_length=10)
    group_id: int = Field(default=None, foreign_key="group.id", ondelete="CASCADE")
    expires: datetime

class GroupCodeInput(SQLModel):
    code: str = Field(min_length=10, max_length=10)


#Friend model
class FriendBase(SQLModel):
    user1: int = Field(foreign_key="user.id", ondelete="CASCADE")   
    user2: int = Field(foreign_key="user.id", ondelete="CASCADE")

class Friend(FriendBase, table=True):
    id: Optional[int] = Field(default=None, primary_key=True)

class FriendIDInput(SQLModel):
    id: int

#Inbox model
class InboxBase(SQLModel):
    type: str                                   
    sending_user: int = Field(foreign_key="user.id", ondelete="CASCADE")   
    receiving_user: int = Field(foreign_key="user.id", ondelete="CASCADE") 
    code: Optional[str] = Field(default=None)  
    
class Inbox(InboxBase, table=True):
    id: Optional[int] = Field(default=None, primary_key=True)


# Question set model
class QuestionSetBase(SQLModel):
    code: str 
    content: bytes
    expires: datetime

class QuestionSet(QuestionSetBase, table=True):
    id: Optional[int] = Field(default=None, primary_key=True)

class QuestionSetInput(SQLModel):
    content: bytes

class QuestionSetCode(SQLModel):
    code: str