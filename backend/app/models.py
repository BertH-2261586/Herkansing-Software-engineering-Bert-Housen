from sqlmodel import SQLModel, Field, Relationship, ForeignKey
from typing import Optional, List

#User Model
class UserBase(SQLModel):
    username: str = Field(min_length=3, max_length=50)
    password: str = Field(min_length=8, max_length=255)

class UserLogin(SQLModel):
    username: str
    password: str

class User(UserBase, table=True):
    id: Optional[int] = Field(default=None, primary_key=True)

    groups_owned: List["Group"] = Relationship(back_populates="owner")

class UserIdInput(SQLModel):
    id: int

#Group Model
class GroupCreate(SQLModel):
    name: str = Field(min_length=3, max_length=50, nullable=False)
    description: Optional[str] = Field(max_length=255)
    owner_id: int = Field(default=None)

class Group(GroupCreate, table=True):
    id: Optional[int] = Field(default=None, primary_key=True)
    owner_id: int = Field(default=None, foreign_key="user.id")


    owner: User = Relationship(back_populates="groups_owned")

class GroupIdInput(SQLModel):
    id: int