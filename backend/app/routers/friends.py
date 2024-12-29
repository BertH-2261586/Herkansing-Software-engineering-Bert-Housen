from fastapi import APIRouter, Depends, HTTPException
from sqlmodel import Session
from ..database import *
from ..models import FriendBase, FriendIDInput
from ..security import UserSessionManager

router = APIRouter()
session_manager = UserSessionManager()

def get_database(session: Session = Depends(get_session)) -> FriendManager:
    return FriendManager(session)

def get_user_database(session: Session = Depends(get_session)) -> UserManager:
    return UserManager(session)

# Add a friend connection to the database
@router.post("/add")
async def create_friend_connection(friend_data: FriendBase, token_data: dict = Depends(session_manager.token_verification), db: FriendManager = Depends(get_database)):
    if token_data["id"] == friend_data.user1:
        db.create_friend_connection(Friend(user1 = friend_data.user1, user2 = friend_data.user2))
    else:
        raise HTTPException(status_code=401, detail="Invalid token")

# Get all the friends of the user
@router.post("/get_friend_usernames")
async def get_all_friends(friend_data: FriendIDInput, token_data: dict = Depends(session_manager.token_verification), 
                        friend_db: FriendManager = Depends(get_database), user_db: UserManager = Depends(get_user_database)) -> dict:
    if token_data["id"] == friend_data.id:  
        friend_IDs = friend_db.get_all_friends(friend_data.id)
        friend_usernames = user_db.get_usernames_from_IDs(friend_IDs)
        return {"IDs": friend_IDs, "friend_usernames": friend_usernames}
    else:
        raise HTTPException(status_code=401, detail="Invalid token")