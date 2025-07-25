{% macro serializer_out(klass, parent=None, comment_tag = "") %}
  {% set sname = (parent.name + "::" if parent else '_') + klass.name %}
  dbOStream& operator<<(dbOStream& stream, const {{sname}}& obj)
  {
    {% for field in klass.fields %}
      {% if field.bitFields %}
        {% if field.numBits == 32 %}
          uint32_t {{field.name}}bit_field;
        {% else %}
          uint64_t {{field.name}}bit_field;
        {% endif %}
        static_assert(sizeof(obj.{{field.name}}) == sizeof({{field.name}}bit_field));
        std::memcpy(&{{field.name}}bit_field, &obj.{{field.name}}, sizeof(obj.{{field.name}}));
        stream << {{field.name}}bit_field;
      {% else %}
        {% if 'no-serial' not in field.flags %}
          stream << {% if field.table %}*{% endif %}obj.{{field.name}};
        {% endif %}
      {% endif %}
    {% endfor %}
    //User Code Begin <<{{ comment_tag }}
    //User Code End <<{{ comment_tag }}
    return stream;
  }
{%- endmacro %}
