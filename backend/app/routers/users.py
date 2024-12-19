from fastapi import APIRouter, Depends, HTTPException, Header
from sqlmodel import Session
from ..database import *
from ..models import User, UserBase, UserLogin, UserIdInput, GroupInviteIdInput
from ..security import PasswordHasher, UserSessionManager

router = APIRouter()
pw_hasher = PasswordHasher()
session_manager = UserSessionManager()

def get_database(session: Session = Depends(get_session)) -> UserManager:
    return UserManager(session)


@router.post("/register/", response_model=dict)
async def register_user(user: UserBase, db: UserManager = Depends(get_database)):
    username_exists = db.get_user(user)

    if username_exists:
        raise HTTPException(status_code=400, detail="Username already taken")
    
    new_user = User(
        username=user.username,
        password=pw_hasher.hash_password(user.password)
    )

    new_user = db.create_user(new_user)
    user_token = session_manager.create_session_token({"id": new_user.id, "username": new_user.username})

    return {"message": "User registered", "id": new_user.id, "username": new_user.username,"token": user_token}


@router.post("/login/", response_model=dict)
async def check_user_login(user: UserLogin, db: UserManager = Depends(get_database)):
    db_user = db.get_user(user)

    if not db_user or not pw_hasher.verify_password(user.password, db_user.password):
        raise HTTPException(status_code=401, detail="Invalid credentials")
    
    user_token = session_manager.create_session_token({"id": db_user.id, "username": db_user.username})

    return {"message": "User logged in", "id": db_user.id, "username":db_user.username, "token": user_token}

@router.post("/remove/", response_model=dict)
async def remove_user(user: UserIdInput, token_data: dict = Depends(session_manager.token_verification), db: UserManager = Depends(get_database)):
    if token_data["id"] == user.id:
        if db.remove_user(user.id):
            return {"message": "user deleted succesfully"}
        else:
            raise HTTPException(status_code=400, detail="Unable to remove user")
    else:
        raise HTTPException(status_code=401, detail="Invalid token")
    
@router.post("/invites/", response_model=dict)
async def get_user_invites(user: UserIdInput, token_data: dict = Depends(session_manager.token_verification), db: UserManager = Depends(get_database)):
    if token_data["id"] == user.id:
        invites = db.get_user_invites(user.id)

        if invites:
            return {"message": "User has invites", "invites": invites}  
        else:
            return {"message": "user has no group invites"}
    else:
        raise HTTPException(status_code=401, detail="Invalid token")

@router.post("/invites/accept", response_model=dict)
async def accept_user_invite(invite: GroupInviteIdInput, token_data: dict = Depends(session_manager.token_verification), db: UserManager = Depends(get_database)):
    if db.has_invite_by_id(token_data["id"], invite.id):
        if db.accept_invite(invite.id):
            return {"message": "Successfully accepted invite"}  
        else:
            return {"message": "Failed to accept invite"}
    else:
        raise HTTPException(status_code=401, detail="Invalid token")

@router.post("/invites/reject", response_model=dict)
async def reject_user_invite(invite: GroupInviteIdInput, token_data: dict = Depends(session_manager.token_verification), db: UserManager = Depends(get_database)):
    if db.has_invite_by_id(token_data["id"], invite.id):
        if db.reject_invite(invite.id):
            return {"message": "Successfully rejected invite"}  
        else:
            return {"message": "Failed to reject invite"}
    else:
        raise HTTPException(status_code=401, detail="Invalid token")

