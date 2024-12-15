from fastapi import APIRouter, Depends, HTTPException
from sqlmodel import Session
from ..database import *

router = APIRouter()

def get_database(session: Session = Depends(get_session)) -> InboxManager:
    return InboxManager(session)

# @router.get("/add", response_model=User)
# async def create_user_route(db: UserManager = Depends(get_database)):
#     return db.create_user(User(username="TestUser", password="blablabla"))
