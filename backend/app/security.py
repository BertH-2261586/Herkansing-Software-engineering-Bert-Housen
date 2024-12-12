from fastapi import Header, HTTPException
from passlib.context import CryptContext
import jwt

#contains functions to hash and verify hashed functions
class PasswordHasher:
    def __init__(self):
        self.context = CryptContext(schemes=["bcrypt"], deprecated="auto")
    
    def hash_password(self, password):
        """hashes passed password"""
        return self.context.hash(password)

    def verify_password(self, password, hashed_password):
        """verifies password with hashed password, returns true if passwords are the same"""
        return self.context.verify(password, hashed_password)

class UserSessionManager:
    def __init__(self):
        self.secret_key = "placeholder"
        self.algo = "HS256"
        self.expiration = 60        #expiration time in minutes
    
    def create_session_token(self, data: dict):
        """Create JWT"""
        token_data = data.copy()
        token = jwt.encode(token_data, self.secret_key, self.algo)
        
        return token
    
    def verify_session_token(self, token):
        """Verify JWT"""
        try:
            token_data = jwt.decode(token, self.secret_key, self.algo)
            return token_data
        except jwt.PyJWTError:
            None
    
    def token_verification(self, token: str = Header(...)) -> dict:
        token_data = self.verify_session_token(token)

        if not token_data or "id" not in token_data:
            raise HTTPException(status_code=401, detail="Invalid token")
        
        return token_data

