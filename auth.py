import time
import requests
import json
import base64

# Replace with your own details
CLIENT_ID = '6e73370bdfd84ccba9c2da88d32d1211'
CLIENT_SECRET = '64066a67b6704a478d69bfa1087609f1'
REFRESH_TOKEN = 'AQAStcLtN-1BrirEi5JkmDM8rXiSzsiXrGspKab5WwEUniZuw2eaBBylHcBXGaHRPHq6y-VhNX0iqk7OawCvDEVajXu6yePkyIgZe6SCP5eEdh2yuq5o2M9oCWqAmi3O28k'
TOKEN_URL = 'https://accounts.spotify.com/api/token'

# Store the token and expiration time
access_token = ''
expires_at = 0

def refresh_token():
    global access_token, expires_at
    
    # Prepare the request headers and body
    auth_header = {
        'Authorization': f'Basic {base64.b64encode(f"{CLIENT_ID}:{CLIENT_SECRET}".encode()).decode()}'
    }
    
    data = {
        'grant_type': 'refresh_token',
        'refresh_token': REFRESH_TOKEN
    }
    
    # Send the POST request to refresh the token
    response = requests.post(TOKEN_URL, headers=auth_header, data=data)
    
    if response.status_code == 200:
        token_data = response.json()
        access_token = token_data['access_token']
        expires_in = token_data['expires_in']
        expires_at = time.time() + expires_in  # Store expiration time
        print(f"Token refreshed! New access token: {access_token}")
    else:
        print(f"Error refreshing token: {response.status_code}, {response.text}")

def get_access_token():
    global access_token, expires_at
    
    # If the token has expired, refresh it
    if time.time() > expires_at:
        print("Access token expired, refreshing...")
        refresh_token()
    
    return access_token

# Example usage
def main():
    access_token = get_access_token()
    print(f"Current access token: {access_token}")
    # Make your API calls here using the access_token

if __name__ == "__main__":
    main()

