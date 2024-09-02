import fnmatch
import os
import datetime
from git import Repo

# Define the blacklist patterns
blacklist = ['*.md']

def gentry(repo):
    # Get the current date and time
    now = datetime.datetime.now()
    date_str = now.strftime('%m/%d/%Y %H:%M:%S')

    changed = [f.a_path for f in repo.index.diff(None) if not any(fnmatch.fnmatch(f.a_path, pattern) for pattern in blacklist)]

    additions = sum(f.diff.b_blob.data_stream.read().count(b'\n') for f in repo.index.diff(None) if f.diff.b_blob)
    deletions = sum(f.diff.a_blob.data_stream.read().count(b'\n') for f in repo.index.diff(None) if f.diff.a_blob)

    changelog_entry = f"# {date_str}\n{len(changed)} files have been changed with {additions} additions and {deletions} deletions\nFiles changed:\n"
    for file in changed:
        changelog_entry += f"* {file}\n"

    return changelog_entry

def main():
    try:
        repo = Repo()
        changelog_entry = gentry(repo)

        with open('CHANGELOG.md', 'a') as f:
            f.write(changelog_entry)

        print("Success")
    except Exception as e:
        print(f"{e}")

if __name__ == "__main__":
    main()