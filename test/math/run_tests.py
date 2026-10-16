from pathlib import Path
import sys
sys.path.insert(0,str(Path(__file__).resolve().parents[1]))
from check_headers import check
from run_chapter import run
if __name__=="__main__":
    check("math")
    run("math")
