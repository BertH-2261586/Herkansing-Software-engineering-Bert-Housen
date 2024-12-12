from fastapi import APIRouter, Depends, HTTPException, Header
from sqlmodel import Session
from ..database import *
from ..models import User, UserBase, UserLogin, UserIdInput
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

    return {"message": "User logged in", "id": db_user.id, "username":db_user.username, "id": db_user.id, "token": user_token}

@router.post("/remove/", response_model=dict)
async def remove_user(user: UserIdInput, token_data: dict = Depends(session_manager.token_verification), db: UserManager = Depends(get_database)):
    if token_data["id"] == user.id:
        if db.remove_user(user.id):
            return {"message": "user deleted succesfully"}
        else:
            raise HTTPException(status_code=400, detail="Unable to remove user")
    else:
        raise HTTPException(status_code=401, detail="Invalid token")