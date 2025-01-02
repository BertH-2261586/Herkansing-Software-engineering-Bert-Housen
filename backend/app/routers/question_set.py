from fastapi import APIRouter, Depends, HTTPException, Body
from sqlmodel import Session
from ..database import *
from ..models import QuestionSet, QuestionSetCode
from ..security import UserSessionManager
from datetime import datetime, timedelta, timezone
import string
import random
from io import BytesIO
from fastapi.responses import StreamingResponse

router = APIRouter()
session_manager = UserSessionManager()

def get_database(session: Session = Depends(get_session)) -> QuestionSetManager:
    return QuestionSetManager(session)

def generateCode(length):
    """Generates a 10 char long code"""
    chars = string.ascii_letters + string.digits
    return ''.join(random.choices(chars, k=length))

# Add a question set to the database
@router.post("/add")
async def add_question_set(zip_file: bytes = Body(...), token_data: dict = Depends(session_manager.token_verification), 
                           db: QuestionSetManager = Depends(get_database)) -> dict:
    generated_code = ""
    # Keep generating a new code until you have one that isnt present in the database
    while True:
        generated_code = generateCode(10)     
        if not db.code_exists(generated_code):
            break

    expiration_time = datetime.now(timezone.utc) + timedelta(days=30)
    db.add_question_set(QuestionSet(code = generated_code, content = zip_file, expires = expiration_time))

    return {"code": generated_code}

# Get the question set given a code
@router.post("/get_question_set")
async def add_question_set(code_input: QuestionSetCode, db: QuestionSetManager = Depends(get_database)):
    try:
        question_set_data = db.get_question_set(code_input.code)
        if question_set_data is None:
            # Return 204 No Content if no data is found
            return {"question_set": None}

        # If data exists, return the zip file
        file_like = BytesIO(question_set_data)
        return StreamingResponse(
            file_like,
            media_type="application/zip",
            headers={"Content-Disposition": f"attachment; filename={code_input.code}.zip"}
        )
    except:
        raise HTTPException(status_code=401, detail="Invalid token")