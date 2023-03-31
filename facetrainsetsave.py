import cv2
import os

# Create the directory to save the images
save_dir = 'face_dataset_mask'
if not os.path.exists(save_dir):
    os.mkdir(save_dir)

# Set up the camera
cap = cv2.VideoCapture(0)

# Set up the face detector
face_cascade = cv2.CascadeClassifier('D:\Workkk\Robot proeject\python\haarcascade_frontalface_default.xml')

# Set the counter for the image filenames
img_counter = 0

# Loop to capture images
while True:
    # Read the frame from the camera
    ret, frame = cap.read()

    # Convert to grayscale
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Detect the face
    faces = face_cascade.detectMultiScale(gray, 1.3, 5)

    # Draw a rectangle around the face and save the image
    for (x, y, w, h) in faces:
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

        # Save the image to the dataset folder
        img_name = os.path.join(save_dir, f'face_{img_counter}.jpg')
        cv2.imwrite(img_name, frame[y:y + h, x:x + w])
        print(f'Saved {img_name}')

        img_counter += 1

    # Display the frame
    cv2.imshow('Capture Face', frame)

    # Exit if 'q' is pressed
    if cv2.waitKey(1) == ord('q'):
        break

# Release the camera and destroy the window
cap.release()
cv2.destroyAllWindows()
