from fastapi import APIRouter, Depends, HTTPException
from sqlmodel import Session
from ..database import *
from ..models import GroupCreate, Group, GroupIdInput
from ..security import UserSessionManager

router = APIRouter()
session_manager = UserSessionManager()

def get_database(session: Session = Depends(get_session)) -> GroupManager:
    return GroupManager(session)


@router.post("/create", response_model=Group)
async def create_group_route(group: GroupCreate, token_data: dict = Depends(session_manager.token_verification), db: GroupManager = Depends(get_database)):
    if token_data["id"] == group.owner_id:
        return db.create_group(Group(name=group.name, description=group.description, owner_id=group.owner_id))
    else:
        raise HTTPException(status_code=401, detail="Invalid token")
    
@router.post("/remove", response_model=dict)
async def remove_group_route(group: GroupIdInput, token_data: dict = Depends(session_manager.token_verification), db: GroupManager = Depends(get_database)):
    if db.is_group_owner(token_data["id"], group.id):
        if db.remove_group(group.id):
            return {"message": "group deleted succesfully"}
        else:
            raise HTTPException(status_code=400, detail="Unable to remove group")
    else:
        raise HTTPException(status_code=401, detail="Invalid token")