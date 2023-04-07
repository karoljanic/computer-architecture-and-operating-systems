#!/bin/sh

# Chuck Norris
chuck_norris_response=$(curl https://api.chucknorris.io/jokes/random)

# Cat Image
cat_url_response=$(curl https://api.thecatapi.com/v1/images/search)
cat_img_url=$(echo $cat_url_response | jq '.[] | .url' | tr -d '"')
curl -s -o cat_img_response.png $cat_img_url

catimg cat_img_response.png -w 200
echo $chuck_norris_response | jq '.value'

rm cat_img_response.png