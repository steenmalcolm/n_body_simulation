#!/bin/bash

# Step 2: Execute the compiled binary
./main.exe

# Check if the binary execution was successful
if [ $? -ne 0 ]; then
    echo "Execution of the binary failed!"
    exit 2
fi

# Step 3: Run the Python script
python3 gen_frames.py

# Check if the Python script execution was successful
if [ $? -ne 0 ]; then
    echo "Execution of the Python script failed!"
    exit 3
fi

# Step 4: Execute the ffmpeg command (modify the command as per your needs)
ffmpeg -y -framerate 10 -i frames/frame_%03d.png -vf "fps=10" -c:v gif simulation.gif



# Check if ffmpeg execution was successful
if [ $? -ne 0 ]; then
    echo "ffmpeg command failed!"
    exit 4
fi

echo "All steps completed successfully!"
