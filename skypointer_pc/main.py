import pygame
import trans
import socket
import time
from pygame.locals import *
import threading
stored_text = ""
starname = ""
star_avaliable = True
def run_socket():
    global stored_text
    global starname
    global star_avaliable
    HOST = "192.168.205.116"
    PORT = 16990
    t = 0
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server:
        print("Socket created")
        server.bind((HOST, PORT))
        print("Server started")
        server.listen()
        print("Waiting for connection...")
        while True:
            conn, addr = server.accept()
            with conn:
                print("Connected by", addr)
                while conn:
                    if stored_text != starname:
                        starname = stored_text
                        altitude, azimuth = trans.get_ninjas_star(starname)
                        if float(altitude) < 0.0 :
                            star_avaliable = False
                        else:
                            star_avaliable = True
                        serverMessage = str(altitude) + " " + str(azimuth)
                        conn.sendall(serverMessage.encode())
                    time.sleep(5)

    print("Server closed")
t = threading.Thread(target = run_socket)
t.start()


# Initialize Pygame
pygame.init()
# Set up the display
screen = pygame.display.set_mode((800, 600))
pygame.display.set_caption("Pygame UI")
# Set up the font
font = pygame.font.Font(None, 32)
# Set up the text input
text_input = pygame.Rect(200, 200, 400, 40)
text = ""
# Game loop
running = True
while running:
    for event in pygame.event.get():
        if event.type == QUIT:
            running = False
        elif event.type == KEYDOWN:
            if event.key == K_BACKSPACE:
                text = text[:-1]
            elif event.key == K_RETURN:
                # Store the text as a string
                stored_text = text
                text = ""
                print(stored_text)
            else:
                text += event.unicode
    # Clear the screen
    screen.fill((255, 255, 255))
    # Render the text input
    pygame.draw.rect(screen, (0, 0, 0), text_input, 2)
    rendered_text = font.render(text, True, (0, 0, 0))
    screen.blit(rendered_text, (text_input.x + 5, text_input.y + 5))

    # Render the additional text
    additional_text = font.render("Please input star", True, (0, 0, 0))
    screen.blit(additional_text, (200, 150))
    if not star_avaliable:
        additional_text = font.render("Star is underground!", True, (0, 0, 0))
        screen.blit(additional_text, (200, 250))
    
    # Update the display
    pygame.display.flip()

# Quit Pygame
pygame.quit()
