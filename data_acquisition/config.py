import yaml

try:
    with open("config.yaml", "r") as stream:
        config = yaml.safe_load(stream)
except yaml.YAMLError as exc:
    print("Cannot parse yaml")
    exit()
except FileNotFoundError:
    print("Cannot find yaml")
    exit()