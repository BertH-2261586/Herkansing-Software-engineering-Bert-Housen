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
    

