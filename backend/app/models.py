import base64
from sqlmodel import SQLModel, Field, Relationship, ForeignKey
from typing import Optional, List, Tuple
from pydantic import BaseModel
from datetime import datetime


# JSON validation models

class QuestionSetShare(BaseModel):
    code: str
    id: int
    friend_ids: list[int]


class UserVakLink(SQLModel, table=True):
    user_id: Optional[int] = Field(default=None, foreign_key="user.id", primary_key=True)
    vak_id: Optional[int] = Field(default=None, foreign_key="vak.id", primary_key=True)
    titel: str = Field(min_length=1, max_length=32)

# User Model
class UserBase(SQLModel):
    username: str = Field(min_length=3, max_length=50)
    password: str = Field(min_length=8, max_length=255)
    isDocent: bool = Field(default=False)

class UserLogin(SQLModel):
    username: str
    password: str

# Extend the userBase with an ID, this represents the User table
class User(UserBase, table=True):
    id: Optional[int] = Field(default=None, primary_key=True)
    groups_owned: List["Group"] = Relationship(back_populates="owner", cascade_delete=True)
    group_invites: List["GroupInvite"] = Relationship(back_populates="invited_user", cascade_delete=True)
    vakken: List["Vak"] = Relationship(back_populates="docenten", link_model=UserVakLink)

class UserIdInput(SQLModel):
    id: int

class UserSearchRequest(SQLModel):
    userID: int
    page: int = 1               # Default page is 1
    search: Optional[str] = None   # Search query is optional

class UsernameInput(SQLModel):
    username: str

#vakken Models
class Vak(SQLModel, table=True):
    id: Optional[int] = Field(default=None, primary_key=True)
    vaknaam: str = Field(min_length=1, max_length=255)
    docenten: List[User] = Relationship(back_populates="vakken", link_model=UserVakLink)
    proefexamens: List["Proefexamen"] = Relationship(back_populates="vak")

#helper models
class VakCreate(SQLModel):
    vaknaam: str

#preofexamen models
class Proefexamen(SQLModel, table=True):
    id: Optional[int] = Field(default=None, primary_key=True)
    data: bytes
    vak_id: Optional[int] = Field(default=None, foreign_key="vak.id")
    vak: Optional[Vak] = Relationship(back_populates="proefexamens")
    score: List["Scores"] = Relationship(back_populates="proefexamen")

    @property
    def data_b64(self) -> str:
        """Return the data as a base64-encoded string."""
        return base64.b64encode(self.data).decode("utf-8")

class Scores(SQLModel, table=True):
    id: Optional[int] = Field(default=None, primary_key=True)
    percent: int
    student: str = Field(min_length=3, max_length=50)
    examen_id: Optional[int] = Field(default=None, foreign_key="proefexamen.id")
    proefexamen: Optional[Proefexamen] = Relationship(back_populates="score")

class ScoreOut(SQLModel):
    id: int
    percent: int
    student: str
    examen_id: int


class ProefexamenIn(SQLModel):
    data: str
    vak_id: int

class ProefexamenOut(SQLModel):
    id: int
    data: str
    score: List[ScoreOut]

#wrapper klasse
class DocentWithTitel(SQLModel):
    username: str
    id: int
    titel: str

class VakWithDocentTitle(SQLModel):
    id: int
    vaknaam: str
    docenten: List[DocentWithTitel]
    proefexamen: List[ProefexamenOut]



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