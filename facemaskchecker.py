import cv2
import numpy as np
import tensorflow as tf


# Load the model
model = tf.keras.models.load_model('D:\Workkk\Robot proeject\python\mask_detection_model.h5')

# Load the cascade
face_cascade = cv2.CascadeClassifier('D:\Workkk\Robot proeject\python\haarcascade_frontalface_default.xml')

# Capture the video
cap = cv2.VideoCapture(0)

while True:
    # Read the frame
    _, img = cap.read()
    
    # Convert the frame to grayscale
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    
    # Detect the faces
    faces = face_cascade.detectMultiScale(gray, 1.1, 4)
    
    # Draw the rectangles around the faces and predict if they are wearing a mask
    for (x, y, w, h) in faces:
        face_img = gray[y:y+w, x:x+w]
        resized = cv2.resize(face_img, (224, 224))
        # Convert grayscale to color image
        resized = cv2.cvtColor(resized, cv2.COLOR_GRAY2BGR)
        normalized = resized / 255.0
        reshaped = np.reshape(normalized, (1, 224, 224, 3)) # change to 3 channels
        result = model.predict(reshaped)
        
        label = np.argmax(result, axis=1)[0]
        
        if label == 0:
            cv2.rectangle(img, (x, y), (x+w, y+h), (0, 255, 0), 2)
            cv2.putText(img, 'MASK', (x, y-10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, (0, 255, 0), 2)
            command = "saimask"
        else:
            cv2.rectangle(img, (x, y), (x+w, y+h), (0, 0, 255), 2)
            cv2.putText(img, 'NO MASK', (x, y-10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, (0, 0, 255), 2)
            command = "maisaimask"
        
    # Show the frame
    cv2.imshow('Face Mask Detection', img)
    
    
    # Exit if the user presses 'q'
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
    
# Release the capture and destroy the window
cap.release()
cv2.destroyAllWindows()
