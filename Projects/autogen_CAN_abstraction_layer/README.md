# Usage for Development
First set up a virtual environment by running


```python
python3 -m venv environment
```

> **NOTE:** If that does not work try `python -m venv environment`.

Next activate the environment. For Linux users:
```python
source environment/bin/activate
```
If you are on Windows: `python -m venv environment`.
```python
./environment/Scripts/activate
```


Then install all the requirements setup by running:

```python
pip install -r requirements.txt
```

Then run `autogenerator.py` inside the `Autogenerator` folder.
