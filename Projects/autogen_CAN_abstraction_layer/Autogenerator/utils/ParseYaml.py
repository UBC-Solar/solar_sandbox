# from utils.FileUtils import _this_file_path
from FileUtils import _this_file_path
from yaml import safe_load, YAMLError
import os


# FILE CONSTANTS
RELATIVE_PATH           = '../..'
YAML_DIR                = 'yamls'
CAN_MSGS_YAML           = 'CAN_msgs.yaml'

FINAL_PATH = f'{RELATIVE_PATH}/{YAML_DIR}/{CAN_MSGS_YAML}'



def _safe_yaml_load(file) -> dict:
    try:
        yaml_data: dict = safe_load(file)
        return yaml_data
    except YAMLError as e:
        print(f"Error reading YAML file: {e}")
        return None
    

# Brief: Read the yaml specified in the yamls folder
# Param: file_path: str - The path to the yaml file

def YAML_read(file_path: str = FINAL_PATH) -> dict:
    # Get the directory I am running in
    this_file_path: str     = _this_file_path()
    yaml_path: str          = os.path.join(this_file_path, file_path)
    
    with open(yaml_path, 'r') as file:
        return _safe_yaml_load(file)
    