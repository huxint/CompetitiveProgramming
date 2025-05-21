#pragma once
#include <bits/stdc++.h>

namespace Geometry {
    template <typename T>
        requires std::is_arithmetic_v<T>
    class Point {
    public:
        Point(T x = {}, T y = {}) : _x(x), _y(y) {}

        T x() const {
            return _x;
        }

        T y() const {
            return _y;
        }

        void set_x(const T &value) {
            _x = value;
        }

        void set_y(const T &value) {
            _y = value;
        }

        void set_unit() {
            *this /= norm();
        }

        Point get_unit() const {
            return Point(*this) /= norm();
        }

        T norm() const {
            return std::sqrt(_x * _x + _y * _y);
        }

        T norm_squared() const {
            return _x * _x + _y * _y;
        }

        T dot(const Point &other) const {
            return _x * other.x() + _y * other.y();
        }

        T cross(const Point &other) const {
            return _x * other.y() - _y * other.x();
        }

        T angle() const {
            return std::atan2(_y, _x);
        }

        T angle(const Point &other) const {
            return std::atan2(cross(other), dot(other));
        }

        void rotate(const T &angle) {
            T cos = std::cos(angle);
            T sin = std::sin(angle);
            _x = _x * cos - _y * sin;
            _y = _x * sin + _y * cos;
        }

        void rotate_degrees(const T &theta) {
            rotate(theta * std::numbers::pi / 180);
        }

        void translate(const T &dx, const T &dy) {
            _x += dx;
            _y += dy;
        }

        void reflect_x() {
            _y = -_y;
        }

        void reflect_y() {
            _x = -_x;
        }

        void reflect() {
            _x = -_x;
            _y = -_y;
        }

        T distance(const T &x, const T &y) const {
            auto dx = this->x() - x;
            auto dy = this->y() - y;
            return std::sqrt(dx * dx + dy * dy);
        }

        T distance(const Point &other) const {
            return distance(other.x(), other.y());
        }

        Point &operator+=(const Point &other) {
            _x += other.x();
            _y += other.y();
            return *this;
        }

        Point &operator-=(const Point &other) {
            _x -= other.x();
            _y -= other.y();
            return *this;
        }

        Point &operator*=(const T &scalar) {
            _x *= scalar;
            _y *= scalar;
            return *this;
        }

        Point &operator/=(const T &scalar) {
            assert(scalar != 0);
            _x /= scalar;
            _y /= scalar;
            return *this;
        }

        friend Point operator+(const Point &lhs, const Point &rhs) {
            return Point(lhs) += rhs;
        }

        friend Point operator-(const Point &lhs, const Point &rhs) {
            return Point(lhs) -= rhs;
        }

        friend Point operator*(const Point &lhs, const T &rhs) {
            return Point(lhs) *= rhs;
        }

        friend Point operator*(const T &lhs, const Point &rhs) {
            return Point(rhs) *= lhs;
        }

        friend Point operator/(const Point &lhs, const T &rhs) {
            return Point(lhs) /= rhs;
        }

        friend Point operator/(const T &lhs, const Point &rhs) {
            return Point(rhs) /= lhs;
        }

        friend constexpr auto operator==(const Point &lhs, const Point &rhs) -> bool {
            return lhs.x() == rhs.x() and lhs.y() == rhs.y();
        }

        friend std::partial_ordering operator<=>(const Point &lhs, const Point &rhs) {
            if (auto cmp = lhs.x() <=> rhs.x(); cmp != 0) {
                return cmp;
            }
            return lhs.y() <=> rhs.y();
        }

        template <typename Istream>
        friend Istream &operator>>(Istream &istream, Point &self) {
            T x, y;
            istream >> x >> y;
            self = std::move(Point(x, y));
            return istream;
        }

        template <typename Ostream>
        friend Ostream &operator<<(Ostream &ostream, const Point &self) {
            ostream << '(' << self.x() << ',' << self.y() << ')';
            return ostream;
        }

    private:
        T _x, _y;
    };

    template <typename T>
    using Line = std::array<Point<T>, 2>;
    template <typename T>
    using Circle = std::pair<Point<T>, T>;
    template <typename T>
    using Polygon = std::vector<Point<T>>;

    template <typename T>
    T distance(const Point<T> &lhs, const Point<T> &rhs) {
        return lhs.distance(rhs);
    }

    template <typename T>
    Point<T> midpoint(const Point<T> &lhs, const Point<T> &rhs) {
        return Point((lhs.x() + rhs.x()) / 2, (lhs.x() + rhs.y()) / 2);
    }

    template <typename T>
    T angle(const Point<T> &lhs, const Point<T> &rhs) {
        return lhs.angle(rhs);
    }

    template <typename T>
    T cross(const Point<T> &lhs, const Point<T> &rhs) {
        return lhs.cross(rhs);
    }

    template <typename T>
    T dot(const Point<T> &lhs, const Point<T> &rhs) {
        return lhs.dot(rhs);
    }

    template <typename T, auto eps>
    bool equals(const Point<T> &lhs, const Point<T> &rhs) {
        return std::abs(lhs.x() - rhs.x()) < eps and std::abs(lhs.y() - rhs.y()) < eps;
    }

    template <typename T, auto eps>
    bool point_on_segment(const Point<T> &point, const Line<T> &line) {
        auto u = line[0] - point;
        auto v = line[1] - point;
        return equals<T, eps>(u.cross(v), 0) and u.dot(v) <= 0;
    }

    template <typename T, auto eps>
    bool point_on_line(const Point<T> &point, const Line<T> &line) {
        auto u = line[0] - point;
        auto v = line[1] - point;
        return equals<T, eps>(u.cross(v), 0);
    }

    template <typename T, auto eps>
    bool line_intersect(const Line<T> &lhs, const Line<T> &rhs) {
        return equals<T, eps>(cross(lhs[0] - rhs[0], rhs[1] - rhs[0]), 0) and equals<T, eps>(cross(lhs[1] - rhs[0], rhs[1] - rhs[0]), 0);
    }

    template <typename T, auto eps>
    bool segment_intersect(const Line<T> &lhs, const Line<T> &rhs) {
        auto [lhs_min_x, lhs_max_x] = std::minmax(lhs[0].x(), lhs[1].x());
        auto [lhs_min_y, lhs_max_y] = std::minmax(lhs[0].y(), lhs[1].y());
        auto [rhs_min_x, rhs_max_x] = std::minmax(rhs[0].x(), rhs[1].x());
        auto [rhs_min_y, rhs_max_y] = std::minmax(rhs[0].y(), rhs[1].y());
        if (lhs_max_x < rhs_min_x or rhs_max_x < lhs_min_x or
            lhs_max_y < rhs_min_y or rhs_max_y < lhs_min_y) {
            return false;
        }
        auto cross1 = cross(lhs[0] - rhs[0], rhs[1] - rhs[0]);
        auto cross2 = cross(lhs[1] - rhs[0], rhs[1] - rhs[0]);
        auto cross3 = cross(rhs[0] - lhs[0], lhs[1] - lhs[0]);
        auto cross4 = cross(rhs[1] - lhs[0], lhs[1] - lhs[0]);
        auto is_cross = [](auto a, auto b) {
            return (a > 0 and b < 0) or (a < 0 and b > 0);
        };
        if (is_cross(cross1, cross2) and is_cross(cross3, cross4)) {
            return true;
        }
        auto check_endpoint = [&](auto cross_val, const Point<T> &point, const Line<T> &line) {
            return equals<T, eps>(cross_val, 0) and point_on_segment<T, eps>(point, line);
        };
        return check_endpoint(cross1, lhs[0], rhs) or check_endpoint(cross2, lhs[1], rhs) or check_endpoint(cross3, rhs[0], lhs) or check_endpoint(cross4, rhs[1], lhs);
    }

    template <typename T>
    T distance(const Point<T> point, const Line<T> &line) {
        auto u = line[2] - line[1];
        assert(u.norm() != 0);
        return std::abs(u.cross(point - line[1])) / u.norm();
    }

    template <typename T>
    T polygon_area(const Polygon<T> &polygon) {
        T area = 0;
        for (std::size_t i = 0; i < polygon.size(); ++i) {
            area += cross(polygon[i], polygon[(i + 1) % polygon.size()]);
        }
        return std::abs(area) / 2;
    }

    template <typename T, auto eps>
    bool point_in_polygon(const Point<T> &point, const Polygon<T> &polygon) {
        bool inside = false;
        for (std::size_t i = 0; i < polygon.size(); ++i) {
            auto j = (i + 1) % polygon.size();
            if (point_on_segment<T, eps>(point, std::array{polygon[i], polygon[j]})) {
                return true;
            }
            if ((polygon[i].y() > point.y()) != (polygon[j].y() > point.y())) {
                inside ^= point.x() <= (polygon[j].x() - polygon[i].x()) * (point.y() - polygon[i].y()) / (polygon[j].y() - polygon[i].y()) + polygon[i].x();
            }
        }
        return inside;
    }

    template <typename T>
    std::optional<Point<T>> line_intersection(const Line<T> &lhs, const Line<T> &rhs) {
        auto v1 = lhs[1] - lhs[0];
        auto v2 = rhs[1] - rhs[0];
        auto denominator = v1.cross(v2);
        if (denominator == 0) {
            return std::nullopt;
        }
        auto _ = cross(rhs[0] - lhs[0], v2) / denominator;
        return Point<T>{lhs[0].x() + _ * v1.x(), lhs[0].y() + _ * v1.y()};
    }

    template <typename T>
    std::optional<std::pair<Point<T>, Point<T>>> circle_intersection(const Circle<T> &lhs, const Circle<T> &rhs) {
        const auto &[center1, r1] = lhs;
        const auto &[center2, r2] = rhs;
        auto D = center2 - center1;
        T distance = D.norm();
        if (distance > r1 + r2 or distance < std::abs(r1 - r2)) {
            return std::nullopt;
        }
        T a = (r1 * r1 - r2 * r2 + distance * distance) / (2 * distance);
        T h = std::sqrt(r1 * r1 - a * a);
        auto p0 = center1 + D * (a / distance);
        auto perpendicular = Point<T>(-D.y(), D.x()) * (h / distance);
        return std::make_pair(p0 + perpendicular, p0 - perpendicular);
    }

    template <typename T, auto eps>
    bool convex_polygon(const Polygon<T> &polygon) {
        if (polygon.size() < 3) {
            return false;
        }
        int orientation = 0;
        for (std::size_t i = 0; i < polygon.size(); ++i) {
            std::size_t j = (i + 1) % polygon.size();
            std::size_t k = (j + 1) % polygon.size();
            auto _ = cross(polygon[j] - polygon[i], polygon[k] - polygon[j]);
            if (equals<T, eps>(_, 0)) {
                continue;
            }
            if (orientation == 0) {
                orientation = _ > 0 ? 1 : -1;
            } else if (orientation * _ < 0) {
                return false;
            }
        }
        return true;
    }

    template <typename T>
    Polygon<T> convex_hull(const Polygon<T> &polygon) {
        if (polygon.size() <= 3) {
            return polygon;
        }
        Polygon<T> pol = polygon;
        std::sort(pol.begin(), pol.end());
        Polygon<T> hull;
        for (int i = 0; i < 2; ++i) {
            auto start = hull.size();
            for (const auto &point : pol) {
                while (hull.size() >= start + 2) {
                    if (cross(hull.back() - hull[hull.size() - 2], point - hull[hull.size() - 2]) <= 0) {
                        break;
                    }
                    hull.pop_back();
                }
                hull.push_back(point);
            }
            hull.pop_back();
            std::reverse(pol.begin(), pol.end());
        }
        return hull;
    }

    template <typename T>
    std::pair<Point<T>, Point<T>> farthest_pair(const Polygon<T> &polygon) {
        auto hull = convex_hull(polygon);
        if (hull.size() == 2) {
            return std::pair{hull[0], hull[1]};
        }
        int p1 = 0, p2 = 1;
        T max_dist = 0;
        std::pair<Point<T>, Point<T>> res;
        for (std::size_t i = 0, j = 1; i < hull.size(); ++i) {
            while (true) {
                auto next_j = (j + 1) % hull.size();
                if (cross(hull[next_j] - hull[i], hull[j] - hull[i]) <= 0) {
                    break;
                }
                j = next_j;
            }
            T dist = (hull[i] - hull[j]).norm_squared();
            if (dist > max_dist) {
                max_dist = dist;
                res = std::pair{hull[i], hull[j]};
            }
        }
        return res;
    }

    template <typename T>
    Point<T> polygon_centroid(const Polygon<T> &polygon) {
        T area = 0;
        T centroid_x{}, centroid_y{};
        for (std::size_t i = 0; i < polygon.size(); ++i) {
            std::size_t j = (i + 1) % polygon.size();
            auto _ = cross(polygon[i], polygon[j]);
            centroid_x += (polygon[i].x() + polygon[j].x()) * _;
            centroid_y += (polygon[i].y() + polygon[j].y()) * _;
            area += _;
        }
        centroid_x /= 3 * area;
        centroid_y /= 3 * area;
        return Point<T>(centroid_x, centroid_y);
    }
}