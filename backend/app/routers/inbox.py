from fastapi import APIRouter, Depends, HTTPException
from sqlmodel import Session
from ..database import *
from ..models import InboxBase,QuestionSetShare, UserIdInput
from ..security import UserSessionManager

router = APIRouter()
session_manager = UserSessionManager()

def get_database(session: Session = Depends(get_session)) -> InboxManager:
    return InboxManager(session)

@router.post("/add/FriendRequests")
async def create_inbox_item(inbox_data: InboxBase, token_data: dict = Depends(session_manager.token_verification), db: InboxManager = Depends(get_database)):
    if token_data["id"] == inbox_data.sending_user:
        db.create_inbox_item(Inbox(type = inbox_data.type, sending_user = inbox_data.sending_user, receiving_user = inbox_data.receiving_user))
    else:
        raise HTTPException(status_code=401, detail="Invalid token")

@router.post("/add/QuestionSets")
async def create_inbox_item(question_set_data: QuestionSetShare, token_data: dict = Depends(session_manager.token_verification), db: InboxManager = Depends(get_database)):
    if token_data["id"] == question_set_data.id:
        for friend in question_set_data.friend_ids:
            db.create_inbox_item(Inbox(type = "question_set", sending_user = question_set_data.id, receiving_user = friend, code=question_set_data.code))
    else:
        raise HTTPException(status_code=401, detail="Invalid token")

@router.post('/GetInboxMessages', response_model = list[dict])
async def get_user_inbox_messages(user: UserIdInput, token_data: dict = Depends(session_manager.token_verification), db: InboxManager = Depends(get_database)):    
    if token_data["id"] == user.id:
        try: 
            # Query the database for the list of users
            return db.get_inbox_items_user(user.id)
        except Exception as e:
            # Catch any other unexpected errors
            print(f"Unexpected error occurred: {str(e)}")
            return {"error": "Unexpected error", "details": str(e)}
    else:
        raise HTTPException(status_code=401, detail="Invalid token")


@router.delete("/remove")
async def remove_inbox_item(request: Request, token_data: dict = Depends(session_manager.token_verification), db: InboxManager = Depends(get_database)):
    if token_data["id"] == int(request.query_params.get('userID')):
        try:
            inbox_message_ID = int(request.query_params.get('inbox_message_id'))  
            deleted = db.remove_inbox_message(inbox_message_ID)
            if not deleted:
                # If the message wasn't found or couldn't be deleted, raise a 404 error
                raise HTTPException(status_code=404, detail="Message not found or already deleted")
        except HTTPException:
            # Log the database error
            print(f"Database error occurred: {str(e)}")
            return {"error": "Database error", "details": str(e)}
        except Exception as e:
            # Catch any other unexpected errors
            print(f"Unexpected error occurred: {str(e)}")
            return {"error": "Unexpected error", "details": str(e)}
    else:
        raise HTTPException(status_code=401, detail="Invalid token")