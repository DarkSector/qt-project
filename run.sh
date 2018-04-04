echo "Creating conda environment"
conda env create -f environment.yml
echo "Switching to conda environment"
source activate qt
echo "Starting the python server"
nohup `which python` python/qtAPIServer/manage.py runserver 0.0.0.0:9000 &
open ./QtFaceRecognition.app