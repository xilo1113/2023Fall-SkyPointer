import requests
import datetime
from astropy.coordinates import EarthLocation,SkyCoord
from astropy.time import Time
from astropy import units as u
from astropy.coordinates import AltAz
name = "Lyra"
#constellation = "Lyra"
def get_ra_dec(name):
    """
    Get the right ascension and declination of a star or constellation.

    Args:
        name (str): The name of the star.
        constellation (str): The name of the constellation.

    Returns:
        tuple: A tuple containing the right ascension and declination of the star or constellation.
    """
    api_url1 = f'https://api.api-ninjas.com/v1/stars?name={name}'
    api_url2 = f'https://api.api-ninjas.com/v1/stars?constellation={name}'
    response1 = requests.get(api_url1, headers={'X-Api-Key': 'D30V9/R9NBt9p2qQs041aA==aQT4RW3c243DM3ZR'})
    response2 = requests.get(api_url2, headers={'X-Api-Key': 'D30V9/R9NBt9p2qQs041aA==aQT4RW3c243DM3ZR'})
    if response2.status_code == requests.codes.ok:
        data2 = response2.json()
        if data2:
            data = data2[0]
            right_ascension = str(data['right_ascension'])
            declination = str(data['declination'])
            print("constellation exist")
        else:
            print("No such constellation")
    if response1.status_code == requests.codes.ok:
        data1 = response1.json()
        if data1:
            data = data1[0]
            right_ascension = str(data['right_ascension'])
            declination = str(data['declination'])
            print("star exist")
        else:
            print("No such star")
    else:
        print("Error:", response1.status_code, response1.text)
    try:
        right_ascension = right_ascension.replace(" ", "")
        declination = declination.replace("°","d")
        declination = declination.replace("′","m")
        declination = declination.replace("″","s")
        declination = declination.replace("\xa0","")
    except:
        print("No such star or constellation")
        print("show default star")
        return "02h31m48.7s","+89d15m50.8s"
        
    return right_ascension, declination
def get_current_time():
    """
    Get the current time as a string.

    Returns:
        str: The current time in the format "YYYY-MM-DD HH:MM:SS".
    """
    now = datetime.datetime.now()
    now = str(now)
    now = now.split(".")
    now = now[0]
    return now
def altaz_converter(ra, dec, currenttime):
    """
    Convert the given celestial coordinates (right ascension and declination) to
    corresponding altitude and azimuth coordinates at the specified time.

    Parameters:
    ra (str): Right ascension of the celestial object in the format 'hh:mm:ss'.
    dec (str): Declination of the celestial object in the format 'dd:mm:ss'.
    currenttime (str): Current time in the format 'yyyy-mm-dd HH:MM:SS'.

    Returns:
    float: Altitude of the celestial object in degrees.
    float: Azimuth of the celestial object in degrees.
    """
    observing_location = EarthLocation(lat='25.0146114', lon='121.54705169', height=8*u.m)  
    observing_time = Time(currenttime) - 8*u.hour
    #print(currenttime)
    aa = AltAz(location=observing_location, obstime=observing_time)
    coord = SkyCoord(ra, dec)
    coord = coord.transform_to(aa)
    alt, az = coord.alt.deg, coord.az.deg  # Convert to float
    return alt, az
def get_ninjas_star(name):
    """
    Get the altitude and azimuth coordinates of a star.

    Parameters:
    name (str): The name of the star.
    constellation (str): The name of the constellation.

    Returns:
    tuple: A tuple containing the altitude and azimuth coordinates of the star.
    """
    now = get_current_time()
    right_ascension, declination = get_ra_dec(name)
    print(right_ascension,declination)
    alt, az = altaz_converter(right_ascension,declination,now)
    print(alt,az)
    return (alt,az)

#print(get_ninjas_star("vega"))