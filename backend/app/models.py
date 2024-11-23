from sqlmodel import SQLModel, Field
from typing import Optional

#User Model
class User(SQLModel, table=True):
    id: Optional[int] = Field(default=None, primary_key=True)
    username: str = Field(min_length=3, max_length=50)
    password: str = Field(min_length=3, max_length=255)
