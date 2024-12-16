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

    groups_owned: List["Group"] = Relationship(back_populates="owner", cascade_delete=True)
    group_invites: List["GroupInvite"] = Relationship(back_populates="invited_user", cascade_delete=True)

class UserIdInput(SQLModel):
    id: int

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



