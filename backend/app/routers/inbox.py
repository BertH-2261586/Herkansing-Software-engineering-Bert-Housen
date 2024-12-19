from fastapi import APIRouter, Depends, HTTPException
from sqlmodel import Session
from ..database import *
from ..models import InboxBase
from sqlalchemy.exc import SQLAlchemyError

router = APIRouter()

def get_database(session: Session = Depends(get_session)) -> InboxManager:
    return InboxManager(session)

@router.post("/add")
async def create_inbox_item(inbox_data: InboxBase, db: InboxManager = Depends(get_database)):
    db.create_inbox_item(Inbox(type = inbox_data.type, sending_user = inbox_data.sending_user, receiving_user = inbox_data.receiving_user))

@router.get('/get_inbox_messages', response_model = list[dict])
async def getUserInboxMessages(request: Request, db: InboxManager = Depends(get_database)):
    # Get the query parameters from the URL
    userID = int(request.query_params.get('ID'))      

    try: 
        # Query the database for the list of users
        return db.get_inbox_items_user(userID)
    except SQLAlchemyError:
        # Log the database error
        print(f"Database error occurred: {str(e)}")
        return {"error": "Database error", "details": str(e)}
    except Exception as e:
        # Catch any other unexpected errors
        print(f"Unexpected error occurred: {str(e)}")
        return {"error": "Unexpected error", "details": str(e)}

@router.delete("/remove")
async def create_inbox_item(request: Request, db: InboxManager = Depends(get_database)):
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
