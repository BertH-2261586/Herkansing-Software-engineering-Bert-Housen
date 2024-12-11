from fastapi import APIRouter, Depends, HTTPException
from sqlmodel import Session
from ..database import *
from ..models import GroupCreate, Group
from ..security import UserSessionManager

router = APIRouter()
session_manager = UserSessionManager()

def get_database(session: Session = Depends(get_session)) -> GroupManager:
    return GroupManager(session)


@router.post("/create", response_model=Group)
async def create_user_route(group: GroupCreate, db: GroupManager = Depends(get_database)):
    return db.create_group(Group(name=group.name, description=group.description, owner_id=group.owner_id))