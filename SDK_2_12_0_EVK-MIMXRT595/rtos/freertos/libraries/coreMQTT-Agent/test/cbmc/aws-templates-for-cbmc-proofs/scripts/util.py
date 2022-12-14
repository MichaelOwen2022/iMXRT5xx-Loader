# Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0

"""Methods of manipulating the templates repository."""

import logging
import os
import shutil

REPOSITORY_TEMPLATES = "template-for-repository"
PROOF_TEMPLATES = "template-for-proof"

PROOF_DIR = "proofs"

# There are some files that we should copy to the project repository rather than
# symlinking. This is because users are expected to modify these files. If the
# files were symlinks, then modifying them would dirty up this submodule, which
# would prevent project owners from cleanly updating it.
COPY_INSTEAD = [
    "Makefile-project-defines",
    "Makefile-project-targets",
    "Makefile-project-testing",
    ".gitignore",
    "run-cbmc-proofs.py",
]

################################################################

def script_dir():
    """Directory containing setup scripts."""

    return os.path.dirname(os.path.abspath(__file__))

def templates_root():
    """Directory containing the AWS-templates-for-CBMC repository."""

    return os.path.dirname(script_dir())

################################################################
# Read configuration information from the standard input

def read_from_stdin():
    return input().strip()

def read_path_from_stdin(description):
    print("What is the path to {}? ".format(description), end="")
    return os.path.abspath(os.path.expanduser(read_from_stdin()))

def read_source_root_path():
    return read_path_from_stdin("the source root")

def read_proof_root_path():
    return read_path_from_stdin("the 'proofs' directory (usually '.')")

def read_litani_path():
    return read_path_from_stdin("the litani script")

def read_source_path():
    return read_path_from_stdin("the source file defining the function")

def read_function_name():
    print("What is the function name? ", end="")
    return read_from_stdin()

def read_project_name():
    print("What is the project name? ", end="")
    return read_from_stdin()

################################################################

def files_under_root(root):
    """The list of files in the filesystem under root."""

    cwd = os.getcwd()
    try:
        os.chdir(root)
        return [os.path.join(path, name)
                for path, _, files in os.walk('.') for name in files]
    finally:
        os.chdir(cwd)


def link_files(name, src, dst):
    """Link file dst/name to file src/name, return number skipped"""

    src_name = os.path.normpath(os.path.join(src, name))
    dst_name = os.path.normpath(os.path.join(dst, name))

    os.makedirs(os.path.dirname(dst_name), exist_ok=True)
    src_link = os.path.relpath(src_name, os.path.dirname(dst_name))

    if os.path.basename(name) in COPY_INSTEAD:
        install_method = ("copy", shutil.copyfile)
        src_link = src_name
    else:
        install_method = ("symlink", os.symlink)

    if os.path.exists(dst_name):
        logging.warning("Skipping %s %s -> %s: file exists",
                        install_method[0], name, src_link)
        return 1

    logging.warning(
        "Creating %s %s -> %s", install_method[0], name, src_link)
    install_method[1](src_link, dst_name)
    return 0

def copy_directory_contents(src, dst):
    """Link the contents of one directory into another."""

    src = os.path.normpath(src)
    dst = os.path.normpath(dst)

    assert os.path.isdir(src)
    assert os.path.isdir(dst)

    skipped = 0
    for name in files_under_root(src):
        name = os.path.normpath(name)
        skipped += link_files(name, src, dst)

    if skipped:
        logging.warning("To overwrite a skipped file, "
                        "delete the file and rerun the script.")

def copy_repository_templates(cbmc_root):
    """Copy the files in the repository template into the CBMC root."""

    copy_directory_contents(os.path.join(templates_root(),
                                         REPOSITORY_TEMPLATES),
                            cbmc_root)
