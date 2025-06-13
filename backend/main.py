from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
import subprocess

app = FastAPI()


class AESRequest(BaseModel):
    mode: str
    input: str
    key: str


@app.post("/aes")
def aes_api(request: AESRequest):
    # Validate input
    if request.mode not in {"encrypt", "decrypt"}:
        raise HTTPException(
            status_code=400,
            detail="Mode must be 'encrypt' or 'decrypt'"
        )

    if len(request.key) > 16:
        raise HTTPException(
            status_code=400,
            detail="Key must be 16 characters or less"
        )
    print(f"Received request: {request}")
    try:
        # Run the compiled C++ binary
        result = subprocess.run(
            ['./build/AES', request.mode, request.input, request.key],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )

        # Handle C++ errors
        if result.returncode != 0:
            raise HTTPException(status_code=500, detail=result.stderr.strip())

        # Return the clean output
        return {"result": result.stdout.strip()}

    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))


@app.get("/")
def read_root():
    return {
        "message": (
            "Welcome to the AES encryption/decryption API. "
            "Use POST /aes with mode, input, and key."
        )
    }
