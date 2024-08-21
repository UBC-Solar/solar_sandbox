# Read the yaml at ../yamls/CAN_msgs
from yaml import safe_load, YAMLError
import os


# FILE CONSTANTS
RELATIVE_PATH           = '..'
YAML_DIR                = 'yamls'
CAN_MSGS_YAML           = 'CAN_msgs.yaml'

FINAL_PATH = f'{RELATIVE_PATH}/{YAML_DIR}/{CAN_MSGS_YAML}'


def _this_file_path():
    return os.path.dirname(os.path.realpath(__file__))


def _safe_yaml_load(file):
    try:
        yaml_data = safe_load(file)
        return yaml_data
    except YAMLError as e:
        print(f"Error reading YAML file: {e}")
        return None
    


def read_yaml():
    # Get the directory I am running in
    this_file_path = _this_file_path()
    yaml_path = os.path.join(this_file_path, FINAL_PATH)
    
    with open(yaml_path, 'r') as file:
        return _safe_yaml_load(file)
    

def main():
    print(read_yaml())


if __name__ == '__main__':
    main()
