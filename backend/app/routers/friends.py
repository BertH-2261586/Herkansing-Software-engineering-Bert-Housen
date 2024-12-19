from fastapi import APIRouter, Depends, HTTPException
from sqlmodel import Session
from ..database import *
from ..models import FriendBase

router = APIRouter()

def get_database(session: Session = Depends(get_session)) -> FriendManager:
    return FriendManager(session)

# Add a friend connection to the database
@router.post("/add")
async def create_friend_connection(friend_data: FriendBase, db: FriendManager = Depends(get_database)):
    db.create_friend_connection(Friend(user1 = friend_data.user1, user2 = friend_data.user2))
