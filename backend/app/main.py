from fastapi import FastAPI
from contextlib import asynccontextmanager
from .database import engine
from .models import SQLModel

from .routers import users, groups,inbox,friends,question_set


# Create the database and its tables
def create_db_and_tables():
    SQLModel.metadata.create_all(engine)

@asynccontextmanager
async def lifespan(app: FastAPI):
    create_db_and_tables()              # initialize the db before server startup
    yield

app = FastAPI(lifespan=lifespan)

# Add user router
app.include_router(users.router, prefix="/user", tags=["user"])
app.include_router(groups.router, prefix="/group", tags=["group"])
app.include_router(inbox.router, prefix="/inbox", tags=["inbox"])
app.include_router(friends.router, prefix="/friend", tags=["friend"])
app.include_router(question_set.router, prefix="/question_set", tags=["question_set"])

@app.get("/")
def read_root():
    return {"Hello": "World"}
