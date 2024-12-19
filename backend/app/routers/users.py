from fastapi import APIRouter, Depends, HTTPException, Request
from sqlmodel import Session
from ..database import *
from ..models import User, UserBase, UserLogin
from ..security import PasswordHasher, UserSessionManager
from sqlalchemy.exc import SQLAlchemyError  

router = APIRouter()
pw_hasher = PasswordHasher()
session_manager = UserSessionManager()

def get_database(session: Session = Depends(get_session)) -> UserManager:
    return UserManager(session)

# Handles the addition of a user
@router.get("/add", response_model=User)
async def create_user_route(db: UserManager = Depends(get_database)):
    return db.create_user(User(username="TestUser", password="blablabla"))

# Handles the registration of a user
@router.post("/register/", response_model=dict)
async def register_user(user: UserBase, db: UserManager = Depends(get_database)):
    username_exists = db.get_user(user)

    # Check if the username is already taken
    if username_exists:
        raise HTTPException(status_code=400, detail="Username already taken")
    
    # If not taken, make a new user object
    new_user = User(
        username=user.username,
        password=pw_hasher.hash_password(user.password)
    )

    # Add the user to the database
    new_user = db.create_user(new_user)
    user_token = session_manager.create_session_token({"id": new_user.id, "username": new_user.username})

    # Return a response message
    return {"message": "User registered", "username": new_user.username,"token": user_token}

# Logs in a user and returns a session token if credentials are valid
@router.post("/login/", response_model=dict)
async def check_user_login(user: UserLogin, db: UserManager = Depends(get_database)):
    db_user = db.get_user(user)

    # Check if the fields entered by the user are correct
    if not db_user or not pw_hasher.verify_password(user.password, db_user.password):
        raise HTTPException(status_code=401, detail="Invalid credentials")
    
    # If the fields are correct make a session token
    user_token = session_manager.create_session_token({"id": db_user.id, "username": db_user.username})

    # Return a response message 
    return {"message": "User logged in", "username":db_user.username, "id": db_user.id, "token": user_token}

# Gets a certain amount of users
@router.get('/getUsers', response_model=dict)
async def getUsersByPage(request: Request, db: UserManager = Depends(get_database)):
    # Get the query parameters from the URL
    current_page = int(request.query_params.get('page', 1))             # Default to page 1 if not provided
    current_page_size = int(request.query_params.get('pageSize', 1))   # Default to page size 10 if not provided
    current_search_query = request.query_params.get('search', None)             # Default to None if no user input provided

    try: 
        # Query the database for the list of users
        total_count, users = db.get_users_by_page(page = current_page, page_size = current_page_size, search_query = current_search_query)
        return {"total_count": total_count, "users": users}
    except SQLAlchemyError:
        # Log the database error
        print(f"Database error occurred: {str(e)}")
        return {"error": "Database error", "details": str(e)}
    except Exception as e:
        # Catch any other unexpected errors
        print(f"Unexpected error occurred: {str(e)}")
        return {"error": "Unexpected error", "details": str(e)}
    
@router.get("/get_user_id", response_model = dict)
async def create_inbox_item(request: Request, db: UserManager = Depends(get_database)):
    username = request.query_params.get('username')
    user_id = db.get_userID_via_username(username = username)
    return {"user_id": user_id}