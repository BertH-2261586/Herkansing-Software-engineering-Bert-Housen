from passlib.context import CryptContext
import jwt

#contains functions to hash and verify hashed functions
class PasswordHasher:
    def __init__(self):
        self.context = CryptContext(schemes=["bcrypt"], deprecated="auto")
    
    # Hashes the given password
    def hash_password(self, password):
        return self.context.hash(password)

    # Verifies password with hashed password
    def verify_password(self, password, hashed_password):
        return self.context.verify(password, hashed_password)

class UserSessionManager:
    def __init__(self):
        self.secret_key = "placeholder"
        self.algo = "HS256"
        self.expiration = 60        # Expiration time in minutes
    
    # Create JWT
    def create_session_token(self, data: dict):
        token_data = data.copy()
        token = jwt.encode(token_data, self.secret_key, self.algo)
        
        return token
    
    # Verify JWT
    def verify_session_token(self, token):
        try:
            token_data = jwt.decode(token, self.secret_key, self.algo)
            return token_data
        except jwt.PyJWTError:
            None
