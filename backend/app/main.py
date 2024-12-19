from fastapi import FastAPI
from contextlib import asynccontextmanager
from .database import engine
from .models import SQLModel
from .routers import users, groups

def create_db_and_tables():
    SQLModel.metadata.create_all(engine)

@asynccontextmanager
async def lifespan(app: FastAPI):
    create_db_and_tables()              #init db before server startup
    yield

app = FastAPI(lifespan=lifespan)

#Add router
app.include_router(users.router, prefix="/user", tags=["user"])
app.include_router(groups.router, prefix="/group", tags=["group"])

@app.get("/")
def read_root():
    return {"Hello": "World"}
