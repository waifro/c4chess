#! /bin/sh

git add -A

read -p "Enter comment: " message
git commit -m "$message"
git push
