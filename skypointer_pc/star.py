import requests
import base64
import datetime
def get_star_data(star_id: str, latitude: float, longitude: float, elevation:int) -> tuple[float,float]:
    "-> (altitude, azimuth)"
    def get_date() -> str:
        now = datetime.datetime.now()
        now = str(now)
        now = now.split(" ")
        now = now[0]
        return now
    def get_time() -> str:
        now = datetime.datetime.now()
        now = str(now)
        now = now.split(" ")
        now = now[1]
        now = now.split(".")
        now = now[0]
        return now
    parameters = f"time={get_time()}&from_date={get_date()}&to_date={get_date()}&latitude={latitude}&longitude={longitude}&elevation={elevation}"
    userpass = "5c5aece0-69d2-4a05-93e4-e331f1045f33:5f3d8016067c92e86807244acf5a9523208aa0ba2be12519424ae40ab0ba9c7f43d53a855dadafb05582b33b65c7432f2ce4395505eae2fc77534fd36b32ba1c5d04ae1b815003d710de1e2fb8688ff68182e1170833d927b8757ce8ac19375c16dff53ebf80788c08bce15700648d12"
    authString = base64.b64encode(userpass.encode()).decode()
    url = f"https://api.astronomyapi.com/api/v2/bodies/positions/{star_id}?{parameters}"
    headers = {"Authorization": f"Basic {authString}"}
    response = requests.get(url, headers=headers)
    data = response.json()
    data = data["data"]["table"]["rows"][0]["cells"][0]['position']['horizontal']
    altitude = data['altitude']['degrees']
    azimuth = data['azimuth']['degrees']
    altitude = float(altitude)
    azimuth = float(azimuth)
    return altitude, azimuth
print(get_star_data("saturn",25.014907,121.534215,0))