from fastapi import APIRouter, Depends, HTTPException
from sqlmodel import Session
from ..database import *
from ..models import FriendBase
from ..security import UserSessionManager

router = APIRouter()
session_manager = UserSessionManager()

def get_database(session: Session = Depends(get_session)) -> FriendManager:
    return FriendManager(session)

# Add a friend connection to the database
@router.post("/add")
async def create_friend_connection(friend_data: FriendBase, token_data: dict = Depends(session_manager.token_verification), db: FriendManager = Depends(get_database)):
    if token_data["id"] == friend_data.user1:
        db.create_friend_connection(Friend(user1 = friend_data.user1, user2 = friend_data.user2))
    else:
        raise HTTPException(status_code=401, detail="Invalid token")
