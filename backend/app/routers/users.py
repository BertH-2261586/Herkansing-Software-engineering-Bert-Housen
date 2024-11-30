from fastapi import APIRouter, Depends, HTTPException
from sqlmodel import Session
from ..database import *
from ..models import User, UserBase
from ..security import PasswordHasher

router = APIRouter()
pw_hasher = PasswordHasher()

def get_database(session: Session = Depends(get_session)) -> UserManager:
    return UserManager(session)


@router.get("/add", response_model=User)
async def create_user_route(db: UserManager = Depends(get_database)):
    return db.create_user(User(username="TestUser", password="blablabla"))


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

    return {"message": "User registered", "id": new_user.id}


@router.post("/login/", response_model=dict)
async def check_user_login(user: UserBase, db: UserManager = Depends(get_database)):
    db_user = db.get_user(user)

    if not user or not pw_hasher.verify_password(user.password, db_user.password):
        raise HTTPException(status_code=401, detail="Invalid credentials")
    
    return {"message": "User logged in", "id": db_user.id}