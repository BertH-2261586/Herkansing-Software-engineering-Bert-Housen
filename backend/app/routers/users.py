from fastapi import APIRouter, Depends, HTTPException
from sqlmodel import Session
from ..database import *
from ..models import User, UserBase, UserLogin, UserIdInput, GroupInviteIdInput, UserSearchRequest, UsernameInput
from ..security import PasswordHasher, UserSessionManager

router = APIRouter()
pw_hasher = PasswordHasher()
session_manager = UserSessionManager()

def get_database(session: Session = Depends(get_session)) -> UserManager:
    return UserManager(session)

def get_friend_database(session: Session = Depends(get_session)) -> FriendManager:
    return FriendManager(session)

def get_inbox_database(session: Session = Depends(get_session)) -> InboxManager:
    return InboxManager(session)

# Handles the addition of a user
@router.get("/add", response_model=User)
async def create_user_route(db: UserManager = Depends(get_database)):
    return db.create_user(User(username="TestUser", password="blablabla"))

# Handles the registration of a user
@router.post("/register/", response_model=dict)
async def register_user(user: UserBase, db: UserManager = Depends(get_database)):
    username_exists = db.get_user(user.username)

    # Check if the username is already taken
    if username_exists:
        raise HTTPException(status_code=400, detail="Username already taken")
    
    # If not taken, make a new user object
    new_user = User(
        username=user.username,
        password=pw_hasher.hash_password(user.password),
        isDocent = user.isDocent
    )

    # Add the user to the database
    new_user = db.create_user(new_user)
    user_token = session_manager.create_session_token({"id": new_user.id, "username": new_user.username, "isDocent": new_user.isDocent})

    return {"message": "User registered", "id": new_user.id, "username": new_user.username,"token": user_token, "isDocent": new_user.isDocent}

# Logs in a user and returns a session token if credentials are valid
@router.post("/login/", response_model=dict)
async def check_user_login(user: UserLogin, db: UserManager = Depends(get_database)):
    db_user = db.get_user(user.username)

    # Check if the fields entered by the user are correct
    if not db_user or not pw_hasher.verify_password(user.password, db_user.password):
        raise HTTPException(status_code=401, detail="Invalid credentials")
    
    # If the fields are correct make a session token
    user_token = session_manager.create_session_token({"id": db_user.id, "username": db_user.username, "isDocent": db_user.isDocent})
    return {"message": "User logged in", "id": db_user.id, "username":db_user.username, "token": user_token, "isDocent": db_user.isDocent}

@router.post("/remove/", response_model=dict)
async def remove_user(user: UserIdInput, token_data: dict = Depends(session_manager.token_verification), db: UserManager = Depends(get_database)):
    if token_data["id"] == user.id:
        if db.remove_user(user.id):
            return {"message": "user deleted succesfully"}
        else:
            raise HTTPException(status_code=400, detail="Unable to remove user")
    else:
        raise HTTPException(status_code=401, detail="Invalid token")
    
@router.post("/invites/", response_model=dict)
async def get_user_invites(user: UserIdInput, token_data: dict = Depends(session_manager.token_verification), db: UserManager = Depends(get_database)):
    if token_data["id"] == user.id:
        invites = db.get_user_invites(user.id)

        if invites:
            return {"message": "User has invites", "invites": invites}  
        else:
            return {"message": "user has no group invites"}
    else:
        raise HTTPException(status_code=401, detail="Invalid token")

@router.post("/invites/accept", response_model=dict)
async def accept_user_invite(invite: GroupInviteIdInput, token_data: dict = Depends(session_manager.token_verification), db: UserManager = Depends(get_database)):
    if db.has_invite_by_id(token_data["id"], invite.id):
        if db.accept_invite(invite.id):
            return {"message": "Successfully accepted invite"}  
        else:
            return {"message": "Failed to accept invite"}
    else:
        raise HTTPException(status_code=401, detail="Invalid token")

@router.post("/invites/reject", response_model=dict)
async def reject_user_invite(invite: GroupInviteIdInput, token_data: dict = Depends(session_manager.token_verification), db: UserManager = Depends(get_database)):
    if db.has_invite_by_id(token_data["id"], invite.id):
        if db.reject_invite(invite.id):
            return {"message": "Successfully rejected invite"}  
        else:
            return {"message": "Failed to reject invite"}
    else:
        raise HTTPException(status_code=401, detail="Invalid token")

# Gets a certain amount of users
@router.post('/getUsers', response_model=dict)
async def getUsersByPage(searching_info: UserSearchRequest, user_db: UserManager = Depends(get_database), 
                        friend_db: FriendManager = Depends(get_friend_database), inbox_db: InboxManager = Depends(get_inbox_database)):
    current_page_size = 10     
    ID = searching_info.userID                                    
    try: 
        # Query the database for the list of users
        total_count, user_IDs, users = user_db.get_users_by_page(userID = ID, page = searching_info.page, 
                                                  page_size = current_page_size, search_query = searching_info.search)
        
        # Check if the users found are already friends
        are_friends = friend_db.are_friends_batch(ID, user_IDs) 

        # Check if the found users already sent a friend request
        sent_friend_request = inbox_db.check_sent_friend_request_batch(ID, user_IDs)

        return {"total_count": total_count, "users": users, "are_friends": are_friends, "sent_friend_requests": sent_friend_request}
    except Exception as e:
        # Catch any other unexpected errors
        print(f"Unexpected error occurred: {str(e)}")
        return {"error": "Unexpected error", "details": str(e)}
    
@router.post("/get_user_id", response_model = dict)
async def create_inbox_item(user: UsernameInput, db: UserManager = Depends(get_database)):
    user_id = db.get_userID_via_username(username = user.username)
    return {"user_id": user_id}
