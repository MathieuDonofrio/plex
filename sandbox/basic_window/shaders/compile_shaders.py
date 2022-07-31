from glob import glob
import os
from typing import List
import hashlib


def file_hash(file_path: str):
    sha1 = hashlib.sha1()
    with open(file_path, 'rb') as file_to_hash:
        while True:
            data = file_to_hash.read(65536)
            if not data:
                break
            sha1.update(data)
    return sha1.hexdigest()


if __name__ == '__main__':

    compile_command = "glslc.exe {} -o {}"

    shader_files = glob('*.comp') + glob('*.frag') + glob('*.vert')

    if not os.path.exists('shader_hashes.txt'):
        open('shader_hashes.txt', 'w').close()

    with open('shader_hashes.txt', 'r') as f:
        old_hashes = {line.split(':')[0]: line[:-1].split(':')[1] for line in f.readlines()}

    current_hashes = {file_name: file_hash(file_name) for file_name in shader_files}

    changed_hashes = {}

    for filename, current_hash in current_hashes.items():
        if filename not in old_hashes or old_hashes[filename] != current_hash:
            changed_hashes[filename] = current_hash

    done_count = 0
    total_count = len(changed_hashes)
    failed_compilation = []
    for filename in changed_hashes.keys():
        done_count += 1
        print(f"Compiling: {filename} - {done_count}/{total_count}")
        result = os.system(compile_command.format(filename, filename + '.spv'))
        if result != 0:
            failed_compilation.append(filename)

    for filename in failed_compilation:
        del changed_hashes[filename]

    updated_hashes = old_hashes | changed_hashes

    for filename in old_hashes.keys():
        if filename not in current_hashes:
            del updated_hashes[filename]

    os.remove('shader_hashes.txt')

    with open('shader_hashes.txt', 'w') as f:
        for filename, file_hash in updated_hashes.items():
            f.write(f"{filename}:{file_hash}\n")
