#include "camera.hh"
#include "matrix.hh"
#include <vector>

void frustum(mygl::matrix4& mat, const float& left,
    const float& right, const float& bottom,
    const float& top, const float& znear, const float& zfar)
{
  std::vector<float> values(16, 0);
  values[0] = (2 * znear) / (right - left);
  values[2] = (right + left) / (right - left);
  values[5] = (2 * znear) / (top - bottom);
  values[6] = (top + bottom) / (top - bottom);
  values[10] = (zfar + znear) / (zfar - znear);
  values[11] = -(2 * zfar * znear) / (zfar - znear);
  values[14] = -1;

  mygl::matrix4 persp_mat(values);

  mat *= persp_mat;
}

void lookat(mygl::matrix4& mat, const float& eyeX,
    const float& eyeY, const float& eyeZ, const float& centerX,
    const float& centerY, const float& centerZ, float upX, float upY,
    float upZ)
{
  Vector3 f = Vector3(centerX - eyeX, centerY - eyeY, centerZ - eyeZ)
	      .get_normalized();
  Vector3 up_norm = Vector3(upX, upY, upZ).get_normalized();
  Vector3 s = f * up_norm;
  Vector3 u = s.get_normalized() * f;

  std::vector<float> values(16, 0);
  values[0] = s.x;
  values[1] = s.y;
  values[2] = s.z;

  values[4] = u.x;
  values[5] = u.y;
  values[6] = u.z;

  values[8] = -f.x;
  values[9] = -f.y;
  values[10] = -f.z;

  values[15] = 1;

  mygl::matrix4 look_mat(values);
  mat *= look_mat;
  mat = mat * mat; 
}
