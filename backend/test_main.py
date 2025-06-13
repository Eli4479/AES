from fastapi.testclient import TestClient
from main import app

client = TestClient(app)


def test_encrypt_decrypt_cycle():
    original = "hello world"
    key = "secret123"

    # Encrypt
    response = client.post("/aes", json={
        "mode": "encrypt",
        "input": original,
        "key": key
    })
    assert response.status_code == 200
    encrypted = response.json()["result"]
    assert isinstance(encrypted, str)
    assert encrypted != original

    # Decrypt
    response = client.post("/aes", json={
        "mode": "decrypt",
        "input": encrypted,
        "key": key
    })
    assert response.status_code == 200
    decrypted = response.json()["result"]
    assert decrypted == original


def test_invalid_mode():
    response = client.post("/aes", json={
        "mode": "badmode",
        "input": "test",
        "key": "key"
    })
    assert response.status_code == 400


def test_long_key():
    response = client.post("/aes", json={
        "mode": "encrypt",
        "input": "test",
        "key": "thiskeyistoolongtobeaccepted"
    })
    assert response.status_code == 400
