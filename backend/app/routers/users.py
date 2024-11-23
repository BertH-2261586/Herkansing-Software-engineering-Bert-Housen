from fastapi import APIRouter, Depends, HTTPException
from sqlmodel import Session
from ..database import *
from ..models import User

router = APIRouter()

@router.get("/add", response_model=User)
async def create_user_route(session: Session = Depends(get_session)):
    return create_user(session, User(username="TestUser", password="blablabla"))
