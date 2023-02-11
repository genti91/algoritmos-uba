#ifndef TP2_H
#define TP2_H
#define _POSIX_C_SOURCE 200809L

#include "postusuario.h"

int prioridad_feed(const post_t *post1, const post_t *post2, int id3);

//Login: debe funcionar en O(1).
//Post: el usuario se encuentra loggeado. Si no fue posible, se avisa al usuario.
bool login(hash_t *usuarios, estado_t *estado);

//Logout: debe funcionar en O(1).
//Pre: el usuario esta loggeado.
//Post: el usuario ya no esta loggeado. Si no estuvo en un principio, se avisa al usuario.
bool logout(estado_t *estado);

//Publicar Post: debe funcionar en O(ulog(p)), siendo u la cantidad de usuarios y p la cantidad de posts que se hayan creado hasta ese momento.
//Pre: el usuario esta loggeado.
//Post: se publico un post con un ID de post en el feed del usuario. Si no estuvo loggeado, se avisa al usuario.
bool publicar(hash_t *usuarios, estado_t estado, vectorc_t *posts);

//Ver próximo post en el feed: debe funcionar en O(log(p)).
//Pre: el usuario esta loggeado.
//Post: se muestra el siguiente post del feed, priorizandose el post del usuario con mayor afinidad con el usuario actual. Si se tienen dos usuarios
//con la misma afinidad, se priorizan los posts creados mas recientemente. Si no estuvo loggeado o no hay mas posts para ver, se avisa al usuario.
//*afinidad en este caso la definimos como la proximidad dentro del archivo de usuarios.
post_t *ver_siguiente_feed(estado_t estado);

//Likear un post: debe funcionar en O(logu).
//Pre: el usuario esta loggeado y esta viendo un post.
//Post: se le sumo un like al post en cuestion. Si este post no existe o no esta loggeado, se le avisa al usuario.
bool likear_post(vectorc_t *posts, estado_t estado);

//Mostrar likes: debe funcionar en O(u).
//Pre: se esta viendo un post.
//Post: se muestra quienes likearon el post. Si este post no existe o no tiene likes, se le avisa al usuario.
void mostrar_likes(vectorc_t *posts);

#endif  // TP2_H
