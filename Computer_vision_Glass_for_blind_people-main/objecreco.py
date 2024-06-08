import cv2
import cvlib as cv
from cvlib.object_detection import draw_bbox
import urllib.request
import numpy as np
import pyttsx3
import easyocr
from googletrans import Translator

url = 'http://172.20.14.6/cam-hi.jpg'
frame_skip = 1  # Process every frame

# Initialize EasyOCR
reader = easyocr.Reader(['en'])

# Initialize voice engine
engine = pyttsx3.init()
voices = engine.getProperty('voices')
engine.setProperty('voice', voices[1].id)  # Change index to change the voice (0 for male, 1 for female)

# Initialize Google Translate API
translator = Translator()

def speak(text):
    engine.say(text)
    engine.runAndWait()

def object_recognition(img):
    bbox, label, conf = cv.detect_common_objects(img)
    img = draw_bbox(img, bbox, label, conf)
    objects = ", ".join(label)
    speak(f"I see {objects}")
    return img

def text_recognition(img):
    results = reader.readtext(img)
    detected_text = ' '.join([result[1] for result in results])
    if detected_text:
        speak(f"I can read: {detected_text}")
    else:
        speak("I couldn't find any text.")
    return img

def run():
    frame_count = 0
    object_recognition_enabled = False
    text_recognition_enabled = False

    cv2.namedWindow("detection", cv2.WINDOW_AUTOSIZE)
    while True:
        img_resp = urllib.request.urlopen(url)
        imgnp = np.array(bytearray(img_resp.read()), dtype=np.uint8)
        im = cv2.imdecode(imgnp, -1)
        im = cv2.resize(im, (320, 240))  # Resize image for faster processing

        if frame_count % frame_skip == 0:
            if object_recognition_enabled:
                im = object_recognition(im)
            elif text_recognition_enabled:
                im = text_recognition(im)

        cv2.imshow('detection', im)
        key = cv2.waitKey(5)
        if key == ord('q'):
            break
        elif key == ord('o'):
            object_recognition_enabled = not object_recognition_enabled
            text_recognition_enabled = False
        elif key == ord('t'):
            text_recognition_enabled = not text_recognition_enabled
            object_recognition_enabled = False

        frame_count += 1

    cv2.destroyAllWindows()

if __name__ == '__main__':
    print("started")
    run()
