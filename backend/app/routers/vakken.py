from fastapi import APIRouter, Depends, HTTPException
from sqlmodel import Session
from ..database import *
from ..models import *
from ..security import UserSessionManager
from fastapi.encoders import jsonable_encoder

router = APIRouter()
session_manager = UserSessionManager()

def get_database(session: Session = Depends(get_session)) -> VakkenManager:
    return VakkenManager(session)

def get_user_database(session: Session = Depends(get_session)) -> UserManager:
    return UserManager(session)

def encode_proefexamen(proefex: Proefexamen) -> ProefexamenOut:
    scOut: List[ScoreOut] = []
    for sc in proefex.score:
        scOut.append(ScoreOut(id=sc.id, student=sc.student, examen_id=sc.examen_id, percent=sc.percent))

    return ProefexamenOut(
        id=proefex.id,
        data=base64.b64encode(proefex.data).decode("utf-8"),
        score=scOut
    )

def get_all_vakken_helper(db: VakkenManager) -> List[VakWithDocentTitle]:
    allVakken = db.get_all_vak_data()
    newAllVakken: List[VakWithDocentTitle] = []
    for singleVak in allVakken:
        encoded_proefexamens = [encode_proefexamen(p) for p in singleVak.proefexamens]
        newVak = VakWithDocentTitle(
            id=singleVak.id,
            vaknaam=singleVak.vaknaam,
            proefexamen=encoded_proefexamens,
            docenten=db.get_users_with_titel_for_vak(singleVak.id)
            )
        newAllVakken.append(newVak)
    #print("DEBUG:", jsonable_encoder(newAllVakken))
    return newAllVakken

@router.get("/getAllVakkenInfo", response_model=dict)
async def get_all_vakken_info(db: VakkenManager = Depends(get_database)):
    docenten = db.get_all_docenten()
    outputDocenten: List[DocentWithTitel] = []
    for docent in docenten:
        outputDocenten.append(DocentWithTitel(
            username=docent.username,
            id=docent.id,
            titel=""
        ))
    return {"vakken": get_all_vakken_helper(db), "docenten": outputDocenten}


@router.post("/addVak", response_model=List[VakWithDocentTitle])
async def add_vak(vakcreate: VakCreate,
                  token_data: dict = Depends(session_manager.token_verification), 
                  db: VakkenManager = Depends(get_database),
                  userdb: UserManager = Depends(get_user_database)):
    
    currentUser: User = userdb.get_user(username=token_data["username"])
    if (currentUser.isDocent):
        db.create_vak(currentUser, vakcreate.vaknaam)
    return get_all_vakken_helper(db)


@router.post("/addDocentToVak", response_model=List[VakWithDocentTitle])
async def add_vak(uservaklink: UserVakLink,
                  token_data: dict = Depends(session_manager.token_verification), 
                  db: VakkenManager = Depends(get_database),
                  userdb: UserManager = Depends(get_user_database)):
    
    currentUser: User = userdb.get_user(username=token_data["username"])
    if (currentUser.isDocent and db.part_of_onderwijsteam(currentUser, uservaklink.vak_id)):
        db.add_docent_to_vak(uservaklink)
    return get_all_vakken_helper(db)
    
        
@router.post("/removeDocentFromVak", response_model=List[VakWithDocentTitle])
async def add_vak(uservaklink: UserVakLink,
                  token_data: dict = Depends(session_manager.token_verification), 
                  db: VakkenManager = Depends(get_database),
                  userdb: UserManager = Depends(get_user_database)):
    
    currentUser: User = userdb.get_user(username=token_data["username"])
    if (currentUser.isDocent and db.part_of_onderwijsteam(currentUser, uservaklink.vak_id)):
        db.remove_docent_from_vak(uservaklink)
    return get_all_vakken_helper(db)   


@router.post("/addExamToVak", response_model=List[VakWithDocentTitle])
async def add_vak(examenIn: ProefexamenIn,
                  token_data: dict = Depends(session_manager.token_verification), 
                  db: VakkenManager = Depends(get_database),
                  userdb: UserManager = Depends(get_user_database)):
    
    currentUser: User = userdb.get_user(username=token_data["username"])
    if (currentUser.isDocent and db.part_of_onderwijsteam(currentUser, examenIn.vak_id)):
        #print("DEBUG: ", examenIn)
        db.add_exam_to_vak(examenIn)
    return get_all_vakken_helper(db)

@router.post("/addScoreToExam", response_model=List[VakWithDocentTitle])
async def add_vak(score: Scores,
                  token_data: dict = Depends(session_manager.token_verification), 
                  db: VakkenManager = Depends(get_database),):  
    db.add_score_to_exam(score)
    return get_all_vakken_helper(db)