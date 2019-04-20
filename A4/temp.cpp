void Model::CylendricalMap() {
  float height = max_z - min_z;
  float r = (max_x - min_x) * 0.5f;
  for (int i = 0; i < numVertices; i++) {
    glm::vec2 tex;
    glm::vec3 pos = vertices[i].getPosition();
    tex.x = acosf(pos.x / r) / (2 * 3.14f);
    tex.y = pos.z;
    vertices[i].setTexCoord(tex);
  }
}
void Model::sphericalPosMap() {
  glm::vec3 center;
  center.x = (max_x + min_x) * 0.5f;
  center.y = (max_y + min_y) * 0.5f;
  center.z = (max_z + min_z) * 0.5f;
  float r = (max_x - min_x) * 0.5f;
  for (int i = 0; i < numVertices; i++) {
    glm::vec3 p = vertices[i].getPosition();
    glm::vec2 tex;
    p.x = p.x - center.x;
    p.y = p.y - center.y;
    p.z = p.z - center.z;
    // glm::normalize(p);
    tex.x = atan2(-p.z, p.x) / (2 * 3.14);
    tex.x+=3.14;
    tex.y = (acos(-p.y / r) / 3.14);
    vertices[i].setTexCoord(tex);
    // cout<<"radius:"<<r<<endl;
  }
}
void Model::planeMap() {
  for (int i = 0; i < numVertices; i++) {
    glm::vec2 tex;
    glm::vec3 pos = vertices[i].getPosition();
    tex.x = (pos.x - min_x) / (max_x - min_x);
    tex.y = (pos.y - min_y) / (max_y - min_y);
    vertices[i].setTexCoord(tex);
  }
}