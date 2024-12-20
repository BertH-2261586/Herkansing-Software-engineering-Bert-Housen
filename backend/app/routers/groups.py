from fastapi import APIRouter, Depends, HTTPException
from sqlmodel import Session
from ..database import *
from ..models import GroupCreate, Group, GroupIdInput, GroupInviteInput, GroupCodeInput
from ..security import UserSessionManager
import random
import string
from datetime import datetime, timedelta, timezone

router = APIRouter()
session_manager = UserSessionManager()

def get_database(session: Session = Depends(get_session)) -> GroupManager:
    return GroupManager(session)

def generateCode(length):
    """Generates a 10 char long code"""
    chars = string.ascii_letters + string.digits
    return ''.join(random.choices(chars, k=length))

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
    
@router.post("/invite", response_model=dict)
async def invite_user_route(invite: GroupInviteInput, token_data: dict = Depends(session_manager.token_verification), db: GroupManager = Depends(get_database)):
    if db.is_group_owner(token_data["id"], invite.group_id):        #only proceed if user sending request is group owner
        if db.invite_user(invite.group_id, invite.user_id):
            return {"message": "user invited succesfully"}
        else:
            raise HTTPException(status_code=400, detail="Unable to invite user to group")
    else:
        raise HTTPException(status_code=401, detail="Invalid token")
    
@router.post("/create_code", response_model=dict)
async def create_code_route(group: GroupIdInput, token_data: dict = Depends(session_manager.token_verification), db: GroupManager = Depends(get_database)):
    if db.is_group_owner(token_data["id"], group.id):        #only proceed if user sending request is group owner
        code = db.get_group_code(group.id)

        if code:
            return {"message": "group already had valid code", "code": code}

        success = False
        code = ""

        while success == False:
            code = generateCode(10)
            
            if not db.get_group_by_code(code):
                success = True
        
        expiration_time = datetime.now(timezone.utc) + timedelta(hours=24)
        db.save_group_code(code, group.id, expiration_time)
        return {"message": "created code succesfully", "code": code}
    else:
        raise HTTPException(status_code=401, detail="Invalid token")

@router.post("/join", response_model=dict)
async def join_group_route(groupInput: GroupCodeInput, token_data: dict = Depends(session_manager.token_verification), db: GroupManager = Depends(get_database)):
    group = db.get_group_by_code(groupInput.code)

    if group:
        db.join_group(token_data["id"], group.id)
        return {"message": "joined group successfully"}

    raise HTTPException(status_code=400, detail="Failed to join group")
